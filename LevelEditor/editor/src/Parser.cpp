//
// Created by otuhii on 11/7/26.
//

#include "../include/Parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "PatternTables.h"

PatternTables Parser::ParseChr(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::binary);

    if (!file)
    {
        std::cerr << "cannot open a file" << std::endl;
        return {};
    }

    return PatternTables{std::vector<uint8_t>{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>() //because by default it is create with endline
    }};
}

void Parser::ExportLevelInfo(const std::string& filepath,  const std::array<Nametable, 2>& nametables)
{
    const bool collisions_exported{
        ExportCollisionTables(filepath, nametables)
    };

    const bool nametables_exported{
        ExportNametables(filepath, nametables)
    };

    if (collisions_exported && nametables_exported)
    {
        std::cout << "Export successful" << std::endl;
    }
    else
    {
        std::cerr << "Export failed" << std::endl;
    }
}

void Parser::ImportLevelInfo(
    const std::string& filepath,
    std::array<Nametable, 2>& nametables
)
{
    const bool collisions_imported{
        ImportCollisionTables(filepath, nametables)
    };

    const bool nametables_imported{
        ImportNametables(filepath, nametables)
    };

    if (collisions_imported && nametables_imported)
    {
        std::cout << "Import successful" << std::endl;
    }
    else
    {
        std::cerr << "Import failed" << std::endl;
    }


}

bool Parser::ExportCollisionTables(
    const std::string& filepath,
    const std::array<Nametable, 2>& nametables
)
{
    std::filesystem::create_directories(filepath);
    std::ofstream file{
        filepath+"collisions.bin",
        std::ios::binary
    };

    if (!file)
    {
        std::cerr << "Failed to open collision export file" << std::endl;
        return false;
    }

    for (const Nametable& nametable : nametables)
    {
        const auto encoded{
            EncodeCollisionTables(nametable)
        };

        file.write(
            reinterpret_cast<const char*>(encoded.data()),
            encoded.size()
        );

        if (!file)
            return false;
    }

    return true;
}

bool Parser::ExportNametables(
    const std::string& filepath,
    const std::array<Nametable, 2>& nametables
)
{
    std::filesystem::create_directories(filepath);

    std::ofstream file{
        filepath+"nametables.bin",
        std::ios::binary
    };

    if (!file)
    {
        std::cerr << "Failed to open nametable export file" << std::endl;
        return false;
    }

    for (const Nametable& nametable : nametables)
    {
        file.write(
            reinterpret_cast<const char*>(nametable.tiles.data()),
            nametable.tiles.size()
        );

        if (!file)
            return false;

        const auto attributes{
            EncodeAttributes(nametable)
        };

        file.write(
            reinterpret_cast<const char*>(attributes.data()),
            attributes.size()
        );

        if (!file)
            return false;
    }

    return true;
}

bool Parser::ImportCollisionTables(
    const std::string& filepath,
    std::array<Nametable, 2>& nametables)
{
    std::ifstream file{
        filepath+"collisions.bin",
        std::ios::binary
    };

    if (!file)
    {
        std::cerr << "Cannot open collision file for importing" << std::endl;
        return false;
    }

    std::array<uint8_t, 240> collisions{};

    for (Nametable& nametable : nametables)
    {
        if (!file.read(
            reinterpret_cast<char*>(collisions.data()),
            collisions.size()
            ))
        {
            return false;
        }

        for (int row{}; row < 30; ++row)
        {
            for (int col{}; col < 32; ++col)
            {
                int binary_index{row * 8 + col/4};

                //One byte contains 4 tiles:
                //Bits:   7 6 | 5 4 | 3 2 | 1 0
                //Tile:    0  |  1  |  2  |  3

                int shift{(3 - col % 4) * 2};

                nametable.collisionTypes[row * 32 + col] =
                    static_cast<CollisionType>((collisions[binary_index] >> shift) & 0x3);
            }
        }
    }

    return true;
}

bool Parser::ImportNametables(
    const std::string& filepath,
    std::array<Nametable, 2>& nametables)
{
    std::ifstream file{
        filepath+"nametables.bin",
        std::ios::binary
    };

    if (!file)
    {
        std::cerr << "Cann't open nametable file for importing" << std::endl;
    }

    for (Nametable& nametable : nametables)
    {
        if (!file.read(
            reinterpret_cast<char*>(nametable.tiles.data()),
            nametable.tiles.size())
            )
        {
            return false;
        }

        std::array<uint8_t, 64> attibutes{};
        if (!file.read(
            reinterpret_cast<char*>(attibutes.data()),
            static_cast<std::streamsize>(attibutes.size()))
            )
        {
            return false;
        }

        for (int row{}; row < 15; ++row)
        {
            for (int col{}; col < 16; ++col)
            {
                int binary_tile_index{
                    (row / 2) * 8 + (col / 2)
                };
                int attibute_index{
                    row * 16 + col
                };

                int local_tile_offset_x{
                    col % 2
                };
                int local_tile_offset_y{
                    row % 2
                };
                int shift{
                    local_tile_offset_y * 2 + local_tile_offset_x
                };

                nametable.palettes[attibute_index] =
                    (attibutes[binary_tile_index] >> (2*shift)) & 0x3;

            }
        }
    }

    return true;
}

std::array<uint8_t, 240> Parser::EncodeCollisionTables(const Nametable &nametable)
{
    std::array<uint8_t, 240> encoded{};

    constexpr int tiles_per_byte{4};

    for (int y{0}; y < Nametable::tile_count_y_; ++y)
    {
        for (int x{0}; x < Nametable::tile_count_x_; ++x)
        {
            const uint32_t tileIndex{
                y * Nametable::tile_count_x_ + x
            };

            const uint32_t byteIndex{
                y * (Nametable::tile_count_x_ / tiles_per_byte) + x / tiles_per_byte
            };

            const int shift{
                (tiles_per_byte - 1 - (x % tiles_per_byte)) * 2
            };

            const uint8_t collisionType{
                static_cast<uint8_t>(nametable.collisionTypes[tileIndex])
            };

            encoded[byteIndex] |=
                static_cast<uint8_t>((collisionType & 0x03) << shift);
        }
    }

    return encoded;
}

std::array<uint8_t, 64> Parser::EncodeAttributes(const Nametable &nametable)
{
    std::array<uint8_t, 64> encoded{};

    constexpr int attributeWidth{8};
    constexpr int paletteRegionsPerRow{16};
    constexpr int paletteRegionRows{15};

    for (int y{0}; y < 8; ++y)
    {
        for (int x{0}; x < 8; ++x)
        {
            uint8_t attribute{0};

            for (int quadrantY{0}; quadrantY < 2; ++quadrantY)
            {
                for (int quadrantX{0}; quadrantX < 2; ++quadrantX)
                {
                    const int paletteX{x * 2 + quadrantX};
                    const int paletteY{y * 2 + quadrantY};

                    if (paletteY >= paletteRegionRows)
                        continue;

                    const uint8_t paletteIndex{
                        nametable.palettes[
                            paletteY * paletteRegionsPerRow + paletteX
                        ]
                    };

                    const int shift{
                        (quadrantY * 2 + quadrantX) * 2
                    };

                    attribute |=
                        static_cast<uint8_t>((paletteIndex & 0x03) << shift);
                }
            }

            encoded[y * attributeWidth + x] = attribute;
        }
    }

    return encoded;
}
