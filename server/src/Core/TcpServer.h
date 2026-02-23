#pragma once
#include <asio.hpp>
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>

#include "ClientSession.h"
#include "Config.h"
#include "Packets/DataManager.h"

class TcpServer {
public:
    bool isActive = false;
    TcpServer(asio::io_context& io, Config& port);
    void Tick();

private:
    void CleanUpDeadClients();
    void OnWrite(std::shared_ptr<ClientSession> client, std::shared_ptr<DataPacket> packet, const asio::error_code& ec, std::size_t bytes);

    void StartAccept();
    void OnAccept(std::shared_ptr<asio::ip::tcp::socket> socket, const asio::error_code& ec);

    void StartRead(std::shared_ptr<ClientSession> client);
    void OnRead(std::shared_ptr<ClientSession> client, const asio::error_code& ec, std::size_t bytes);
    void ReadPacketBody(std::shared_ptr<ClientSession> client, uint8_t packeType);
    void OnReadPacketBody(std::shared_ptr<ClientSession> client, uint8_t packetType, size_t bodySize, const asio::error_code& ec, std::size_t bytes);

    void SendFullSnapshot(std::shared_ptr<ClientSession> client);
    void SendNextPacket(std::shared_ptr<ClientSession> client);

    asio::io_context& io_ctx;
    asio::ip::tcp::acceptor acceptor;
    Config& config;

    std::vector<std::shared_ptr<ClientSession>> clients;
    std::mutex clients_mutex;
    DataManager data_manager_;
};
