#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct PlayerState {
    int32_t id;
    float x, y, z;
};
#pragma pack(pop)
