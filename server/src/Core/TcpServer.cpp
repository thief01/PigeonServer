#include "TcpServer.h"

#include "Logger/Logger.h"
#include "Packets/Packet.h"
#include "ClientSession.h"

TcpServer::TcpServer(asio::io_context &io, Config &config)
    : io_ctx(io),
      acceptor(io_ctx, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), config.port)),
      config(config) {
    isActive = true;
    StartAccept();
    LOG_INFO("Server started!");
}

void TcpServer::Tick() {
    CleanUpDeadClients();

    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto &client: clients) {
        if (!client->isAlive || !client->socket->is_open()) {
            continue;
        }

        auto datas = data_manager_.GetDirtyData();

        if (datas.empty()) continue;

        {
            std::lock_guard<std::mutex> qlock(client->sendQueueMutex);
            for (auto &data : datas) {
                client->sendQueue.push(std::make_shared<DataPacket>(data));
            }
        }

        if (!client->isWriting) {
            SendNextPacket(client);
        }
    }
}

void TcpServer::CleanUpDeadClients() {
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto rm = std::remove_if(clients.begin(), clients.end(),
                             [](auto &c) { return !c->isAlive || !c->socket->is_open(); });
    clients.erase(rm, clients.end());
}

void TcpServer::OnWrite(std::shared_ptr<ClientSession> client, std::shared_ptr<DataPacket> packet, const asio::error_code &ec, std::size_t bytes) {
    if (ec) {
        LOG_WARN("Client disconnected or read error: " + ec.message());
        client->isAlive = false;
        client->socket->close();
    }

    LOG_INFO("Sent");
    SendNextPacket(client);
}

void TcpServer::StartAccept() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(io_ctx);
    auto token =std::bind(&TcpServer::OnAccept, this, socket, std::placeholders::_1);
    acceptor.async_accept(*socket,token);
}

void TcpServer::OnAccept(std::shared_ptr<asio::ip::tcp::socket> socket, const asio::error_code &ec) {
    if (!ec) {
        LOG_INFO("Client connected!");
        auto session = std::make_shared<ClientSession>();
        session->socket = socket;

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(session);
        }
        StartRead(session);
        SendFullSnapshot(session);
    }
    StartAccept();
}

void TcpServer::StartRead(std::shared_ptr<ClientSession> client) {
    client->recvBuffer.resize(sizeof(PacketHeader));
    auto token = std::bind(&TcpServer::OnRead, this, client, std::placeholders::_1, std::placeholders::_2);

    asio::async_read(*client->socket, asio::buffer(client->recvBuffer), token);
}

void TcpServer::OnRead(std::shared_ptr<ClientSession> client, const asio::error_code &ec, std::size_t bytes) {
    if (ec) {
        LOG_WARN("Client disconnected or read error: " + ec.message());
        client->isAlive = false;
        client->socket->close();
        return;
    }

    if (bytes == sizeof(PacketHeader)) {
        uint8_t packetType = client->recvBuffer[0];
        ReadPacketBody(client, packetType);
    } else {
        LOG_WARN("Wrong packet size " + std::to_string(client->recvBuffer.size()));
    }
}

void TcpServer::ReadPacketBody(std::shared_ptr<ClientSession> client, uint8_t packetType) {
    size_t bodySize = 0;

    switch (packetType) {
        case 0: bodySize = 0;
            break;
        case 1: bodySize = sizeof(DataPacket);
            break;
        default:
            LOG_WARN("Unknown packet type: " + std::to_string(packetType));
            StartRead(client);
            return;
    }

    client->recvBuffer.resize(bodySize);

    auto token = std::bind(&TcpServer::OnReadPacketBody, this, client, packetType, bodySize, std::placeholders::_1, std::placeholders::_2);
    asio::async_read(*client->socket, asio::buffer(client->recvBuffer), token);
}

void TcpServer::OnReadPacketBody(std::shared_ptr<ClientSession> client, uint8_t packetType, size_t bodySize, const asio::error_code &ec, std::size_t bytes) {
    if (ec) {
        LOG_WARN("Client disconnected or read error: " + ec.message());
        client->isAlive = false;
        client->socket->close();
        return;
    }

    if (bytes != bodySize) {
        LOG_WARN("Received wrong packet size for type " + std::to_string(packetType));
        StartRead(client);
        return;
    }


    switch (packetType) {
        case 0: break;
        case 1:
            LOG_INFO("Data packet received!");
            DataPacket packet;
            std::memcpy(&packet, client->recvBuffer.data(), sizeof(DataPacket));
            data_manager_.Upsert(packet);
            break;
    }
    StartRead(client);
}

void TcpServer::SendFullSnapshot(std::shared_ptr<ClientSession> client) {

    LOG_INFO("Sending full snapshot")
    auto datas = data_manager_.GetAll();
    {
        std::lock_guard lock(client->sendQueueMutex);
        for (auto data : datas) {
            client->sendQueue.push(std::make_shared<DataPacket>(data));
        }
    }

    if (!client->isWriting) {
        SendNextPacket(client);
    }
}

void TcpServer::SendNextPacket(std::shared_ptr<ClientSession> client) {
    std::unique_lock<std::mutex> lock(client->sendQueueMutex);

    if (client->sendQueue.empty()) {
        client->isWriting =false;
        return;
    }

    auto packet = client->sendQueue.front();
    client->sendQueue.pop();
    lock.unlock();

    client->isWriting = true;
    auto token = std::bind(&TcpServer::OnWrite, this, client, packet, std::placeholders::_1, std::placeholders::_2);
    asio::async_write(*client->socket, asio::buffer(packet.get(), sizeof(DataPacket)), token);
}