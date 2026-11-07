//
// Created by otuhii on 11/7/26.
//

#include "../include/PatternTables.h"

#include <iostream>

PatternTables::Tile PatternTables::DecodeTile(std::size_t pattern_table_index, std::size_t tile_index) const
{
    if (pattern_table_index >= 2 || tile_index >= 256)
    {
        std::cerr << "Wrong pattern table index or tile index for tile decoding" << std::endl;
        return {};
    }

    const std::size_t offset {
        pattern_table_index * 4096 + tile_index * 16
    };

    Tile tile{};

    for (int tile_row{ 0 }; tile_row < 8; ++tile_row)
    {
        std::uint8_t low {binary_data_[offset + tile_row]};
        std::uint8_t high {binary_data_[offset + tile_row + 8]};

        for (int tile_col {0}; tile_col < 8; ++tile_col)
        {
            const int bit {7 - tile_col};

            const std::uint8_t lowBit {static_cast<uint8_t>((low >> bit) & 1)};
            const std::uint8_t highBit {static_cast<uint8_t>((high >> bit) & 1)};

            tile.pixels[tile_row * 8 + tile_col] = lowBit | (highBit << 1);
        }
    }

    return tile;

}
