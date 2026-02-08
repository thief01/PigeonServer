//
// Created by pst12 on 7.02.2026.
//

#ifndef PIGEON_DATAMANAGER_H
#define PIGEON_DATAMANAGER_H
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <unordered_map>
#include <unordered_set>

#include "Packet.h"


class DataManager {

public:
    void Upsert(const DataPacket& packet);
    void Remove(uint32_t ownerId, uint32_t dataType);

    std::optional<DataPacket> Get(uint32_t ownerId, uint32_t dataType) const;
    std::vector<DataPacket> GetAll() const;
    std::vector<DataPacket> GetDirtyData();
    std::vector<DataPacket> GetUpdatedSince(uint32_t timestamp) const;

private:
    std::unordered_map<PacketKey, DataPacket, PacketKeyHash> packets_;
    std::unordered_set<PacketKey, PacketKeyHash> dirtyKeys_;
    mutable std::shared_mutex mutex_;
};


#endif //PIGEON_DATAMANAGER_H