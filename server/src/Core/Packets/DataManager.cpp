//
// Created by pst12 on 7.02.2026.
//

#include "DataManager.h"


void DataManager::Upsert(const DataPacket &packet) {
    std::unique_lock lock(mutex_);

    PacketKey key{packet.ownerId, packet.dataType};
    packets_[key] = packet;
    dirtyKeys_.insert(key);
}

void DataManager::Remove(uint32_t ownerId, uint32_t dataType) {
    std::unique_lock lock(mutex_);
    packets_.erase(PacketKey{ownerId, dataType});
}

std::optional<DataPacket> DataManager::Get(uint32_t ownerId, uint32_t dataType) const {
    std::shared_lock lock(mutex_);

    PacketKey key{ownerId, dataType};
    auto it = packets_.find(key);
    if (it == packets_.end())
        return std::nullopt;
    return it->second;
}

std::vector<DataPacket> DataManager::GetAll() const {
    std::shared_lock lock(mutex_);
    std::vector<DataPacket> result;
    result.reserve(packets_.size());
    for (auto& [_, packet] : packets_) {
        result.push_back(packet);
    }
    return result;
}

std::vector<DataPacket> DataManager::GetDirtyData() {
    std::lock_guard lock(mutex_);
    std::vector<DataPacket> result;
    result.reserve(dirtyKeys_.size());
    for (auto& key: dirtyKeys_) {
        result.push_back(packets_[key]);
    }
    dirtyKeys_.clear();
    return result;
}

std::vector<DataPacket> DataManager::GetUpdatedSince(uint32_t timestamp) const {
    std::shared_lock lock(mutex_);

    std::vector<DataPacket> result;
    for (auto& [_, packet] : packets_) {
        if (packet.timeStamp > timestamp) {
            result.push_back(packet);
        }
    }
    return result;
}
