//
// Created by otuhii on 11/21/26.
//

#include "EditorRenderer.h"

void EditorRenderer::DrawPatternTable(
    uint8_t pattern_table_index,
    const EditorLayout& layout,
    const BackgroundPalettes& palettes,
    const PatternTables& pattern_tables
)
{
    const Point2f start_pos{
        layout.patternMenu.left,
        layout.patternMenu.top
    };

    const Point2f end_pos{
        layout.patternMenu.left+layout.patternMenu.width,
        layout.patternMenu.top+layout.patternMenu.height
    };

    utils::SetColor(0.f, 0.f, 0.f, 0.4f);
    utils::FillRect(0.f, 0.f, layout.window_width, layout.window_height);

    const int y_tile_count{16};
    const int x_tile_count{16};


    const float width{end_pos.x - start_pos.x};
    const float height{end_pos.y-start_pos.y};

    const float tile_width{width/x_tile_count};
    const float tile_height{height/y_tile_count};

    for (int tile_row{}; tile_row < y_tile_count; ++tile_row)
    {
        for (int tile_col{}; tile_col < x_tile_count; ++tile_col)
        {
            const uint8_t tileIndex{
                static_cast<uint8_t>(tile_row * x_tile_count + tile_col)
            };

            const Point2f tilePos{
                start_pos.x + static_cast<float>(tile_col) * tile_width,
                start_pos.y + static_cast<float>(tile_row) * tile_height
            };

            DrawTile(
                pattern_table_index,
                tileIndex,
                tilePos,
                tile_width,
                tile_height,
                palettes.palettes[0],
                pattern_tables
            );
        }
    }

}

void EditorRenderer::DrawButtons(const EditorLayout &layout)
{
    utils::SetColor(1.f, 0.f, 0.f, 1.f);
    utils::FillRect(layout.loadButton);
    utils::SetColor(0.f, 1.f, 0.f, 1.f);
    utils::FillRect(layout.saveButton);
    utils::SetColor(0.f, 0.f, 1.f, 1.f);
    utils::FillRect(layout.patternButton);
}

void EditorRenderer::DrawNametable(
    const Nametable& nametable,
    const Rectf& nametable_bounds,
    uint8_t current_pattern_table,
    const BackgroundPalettes& palettes,
    const PatternTables& pattern_tables
)
{
    const float
            tile_width{nametable_bounds.width / Nametable::tile_count_x_};
    const float
        tile_height{nametable_bounds.height / Nametable::tile_count_y_};

    const int
        palette_regions_per_row{16};

    for (int row{0}; row < Nametable::tile_count_y_; ++row)
    {
        for (int col{0}; col < Nametable::tile_count_x_; ++col)
        {
            const Point2f draw_point{
                nametable_bounds.left + static_cast<float>(col) * tile_width,
                nametable_bounds.top + static_cast<float>(row) * tile_height
            };

            // Which 2×2 region contains this tile?
            const int palette_x{col / 2};
            const int palette_y{row / 2};

            // Select palette index (0–3)
            const uint8_t palette_index{
                nametable.palettes[
                    palette_y * palette_regions_per_row + palette_x
                ]
            };

            // Get the four actual NES colors
            const Palette& palette{
                palettes.palettes[palette_index]
            };

            DrawTile(
                current_pattern_table,
                nametable.tiles[row * Nametable::tile_count_x_ + col],
                draw_point,
                tile_width,
                tile_height,
                palette,
                pattern_tables
            );
        }
    }
}

void EditorRenderer::DrawCollisionOverlay(
    const Nametable &nametable,
    const Rectf &bounds
)
{
    const float tileWidth{bounds.width / Nametable::tile_count_x_};
    const float tileHeight{bounds.height / Nametable::tile_count_y_};

    for (int y{0}; y < Nametable::tile_count_y_; ++y)
    {
        for (int x{0}; x < Nametable::tile_count_x_; ++x)
        {
            const auto type{
                nametable.collisionTypes[y * Nametable::tile_count_x_ + x]
            };

            switch (type)
            {
                case CollisionType::Empty:
                    continue;

                case CollisionType::Solid:
                    utils::SetColor(0.f, 0.4f, 1.f, 0.4f);
                    break;

                case CollisionType::Spikes:
                    utils::SetColor(1.f, 0.f, 0.f, 0.4f);
                    break;

                case CollisionType::Special:
                    utils::SetColor(0.7f, 0.f, 1.f, 0.4f);
                    break;
            }

            utils::FillRect(
                bounds.left + static_cast<float>(x) * tileWidth,
                bounds.top + static_cast<float>(y) * tileHeight,
                tileWidth,
                tileHeight
            );
        }
    }
}


void EditorRenderer::DrawTile(
    uint8_t pattern_table_index,
    uint8_t tile_index,
    const Point2f& startPos,
    float width, float height,
    const Palette& palette,
    const PatternTables& pattern_tables
)
{
    const int tile_resolution{8};

    const float tile_pixel_width{width/tile_resolution};
    const float tile_pixel_height{height / tile_resolution};

    const PatternTables::Tile tile {
        pattern_tables.DecodeTile(pattern_table_index, tile_index)
    };

    for (int pixel_row{0}; pixel_row < tile_resolution; ++pixel_row)
    {
        for (int pixel_col{0}; pixel_col < tile_resolution; ++pixel_col)
        {
            const uint8_t pixelIndex{
                tile.pixels[pixel_row * tile_resolution + pixel_col]
            };

            const uint8_t nesColor{
                palette[pixelIndex]
            };

            const Color4f color{
                UserUtils::GetNesColor(nesColor)
            };

            utils::SetColor(color);

            Point2f position{
                startPos.x + static_cast<float>(pixel_col) * tile_pixel_width,
                startPos.y + static_cast<float>(pixel_row) * tile_pixel_height
            };

            utils::FillRect(
                position,
                tile_pixel_width,
                tile_pixel_height
            );
        }
    }

}
