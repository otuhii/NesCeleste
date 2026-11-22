//
// Created by otuhii on 11/22/26.
//

#ifndef LEVELEDITOR_LEVELDATA_H
#define LEVELEDITOR_LEVELDATA_H
#include <array>
#include <cstdint>


enum class CollisionType : uint8_t
{
    Empty   = 0,
    Solid   = 1,
    Spikes  = 2,
    Special = 3
};

struct Nametable
{
    std::array<uint8_t, 960> tiles;    // 960 values, each 0–255
    std::array<CollisionType, 960> collisionTypes{};
    std::array<uint8_t, 240> palettes; // 240 values, each 0–3

    inline static uint32_t const tile_count_x_{32};
    inline static uint32_t const tile_count_y_{30};
};

struct Level
{
    std::array<Nametable, 2> nametables{};
};

#endif //LEVELEDITOR_LEVELDATA_H


