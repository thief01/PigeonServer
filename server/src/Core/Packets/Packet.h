//
// Created by pst12 on 7.02.2026.
//

#ifndef PIGEON_PACKET_H
#define PIGEON_PACKET_H

#include <cstdint>
#include <array>

constexpr size_t MAX_PACKET_SIZE = 256;

struct PacketKey {
    uint32_t ownerId;
    uint32_t dataType;

    bool operator==(const PacketKey &other) const {
        return ownerId == other.ownerId && dataType == other.dataType;
    }
};

struct PacketKeyHash {
    std::size_t operator()(const PacketKey& k) const {
        return (static_cast<size_t>(k.ownerId) << 32) ^ k.dataType;
    }
};

#pragma pack(push, 1)
struct PacketHeader {
    uint8_t packetType;
};

struct DataPacket {
    uint32_t ownerId;
    uint32_t dataType;
    uint32_t timeStamp;
    std::array<uint8_t, MAX_PACKET_SIZE> data;
};
#pragma pack(pop)

#endif //PIGEON_PACKET_H