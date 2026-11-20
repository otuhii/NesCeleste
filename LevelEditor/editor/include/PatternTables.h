//
// Created by otuhii on 11/7/26.
//

#ifndef LEVELEDITOR_PatternTables_H
#define LEVELEDITOR_PatternTables_H
#include <cinttypes>
#include <vector>
#include <array>

class PatternTables {
public:
    struct Tile
    {
        std::array<std::uint8_t, 64> pixels{};
    };

    PatternTables()=default;
    PatternTables(std::vector<uint8_t> binary_data)
        :binary_data_(std::move(binary_data)){}

    [[nodiscard]]
    Tile DecodeTile(std::size_t pattern_table_index, std::size_t tile_index) const;

private:
    std::vector<uint8_t> binary_data_;
};


#endif //LEVELEDITOR_PatternTables_H
