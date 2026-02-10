//
// Created by pst12 on 7.02.2026.
//

#ifndef PIGEON_CLIENTSESSION_H
#define PIGEON_CLIENTSESSION_H
#include <memory>
#include <queue>
#include <asio/ip/tcp.hpp>

#include "Packets/Packet.h"


struct ClientSession {
    std::shared_ptr<asio::ip::tcp::socket> socket;
    uint32_t ownerId;
    std::atomic<bool>isAlive{true};
    std::atomic<bool> isWriting{false};

    std::vector<uint8_t> recvBuffer;
    std::queue<std::shared_ptr<DataPacket>> sendQueue;
    std::mutex sendQueueMutex;
};

#endif //PIGEON_CLIENTSESSION_H