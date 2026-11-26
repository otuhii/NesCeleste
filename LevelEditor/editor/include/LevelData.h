//
// Created by otuhii on 11/22/26.
//

#ifndef LEVELEDITOR_LEVELDATA_H
#define LEVELEDITOR_LEVELDATA_H
#include <array>
#include <cassert>
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
    static constexpr uint32_t defalt_nametable_index_{0};
    static constexpr uint32_t tile_count_x_{32};
    static constexpr uint32_t tile_count_y_{30};
    static constexpr uint32_t tile_count_{tile_count_x_ * tile_count_y_};

    std::array<uint8_t, tile_count_> tiles;    // 960 values, each 0–255
    std::array<CollisionType, tile_count_> collisionTypes{};
    std::array<uint8_t, 240> palettes; // 240 values, each 0–3

    void EmptyContents()
    {
        for (std::size_t index{}; index < tiles.size(); ++index)
        {
            tiles[index] = defalt_nametable_index_;
            collisionTypes[index] = CollisionType::Empty;
        }
    }
};

struct Level
{
    std::array<Nametable, 2> nametables{};
};

#endif //LEVELEDITOR_LEVELDATA_H


