//
// Created by otuhii on 11/21/26.
//

#include "LevelEditor.h"
#include "Parser.h"


void LevelEditor::Start(
    const std::string& pattern_table_filepath,
    float window_width,
    float window_height,
    float pattern_table_menu_size,
    const std::string& import_export_filepath
)
{
    InitializeEditorLayout(window_width, window_height, pattern_table_menu_size);

    //expecting 8kb patterns that combines two pattern tables
    pattern_tables_ =
        Parser::ParseChr(pattern_table_filepath);

    import_export_filepath_ = import_export_filepath;
}

void LevelEditor::Draw() const
{
    EditorRenderer::DrawButtons(layout_);

    for (size_t index{0}; index < level_.nametables.size(); ++index)
    {
        EditorRenderer::DrawNametable(
            level_.nametables[index],
            layout_.nametable_bounds[index],
            state_.selectedPatternTable,
            background_palettes_,
            pattern_tables_
        );
    }

    if (state_.tool == EditorTool::Collisions ||
        state_.showCollisions)
    {
        for (size_t index{0}; index < level_.nametables.size(); ++index)
        {
            EditorRenderer::DrawCollisionOverlay(
                level_.nametables[index],
                layout_.nametable_bounds[index]
            );
        }
    }

    if (state_.stage == EditorStage::PickPattern)
    {
        EditorRenderer::DrawPatternTable(
            state_.selectedPatternTable,
            layout_,
            background_palettes_,
            pattern_tables_
        );
    }
}

void LevelEditor::OnMouseDown(const SDL_MouseButtonEvent &event)
{
    const auto command{
        EditorInputHandler::OnMouseDown(state_, layout_, event)
    };

    if (command.has_value())
    {
        ExecuteCommand(command.value());
    }
}

void LevelEditor::OnKeyUp(SDL_Keycode key)
{
    const auto command{
        EditorInputHandler::OnKeyUp(state_, key)
    };

    if (command.has_value())
    {
        ExecuteCommand(command.value());
    }
}


void LevelEditor::ExecuteCommand(const EditorCommand &command)
{
    if (const auto* change_tile{std::get_if<ChangeTile>(&command)})
    {
        Nametable& nametable{
            level_.nametables[change_tile->nametable_index]
        };

        const uint32_t index{
            change_tile->y * Nametable::tile_count_x_ + change_tile->x
        };

        nametable.tiles[index] = change_tile->tileIndex;
    }
    else if (const auto* cycle_palette{std::get_if<CyclePalette>(&command)})
    {
        UserUtils::CyclePalette(
            level_.nametables[cycle_palette->nametable_index],
            cycle_palette->x,
            cycle_palette->y
        );
    }
    else if (const auto* cycle_collision{std::get_if<CycleCollision>(&command)})
    {
        UserUtils::CycleCollision(
            level_.nametables[cycle_collision->nametable_index],
            cycle_collision->x,
            cycle_collision->y
        );
    }
    else if (const auto* change_editor_stage{std::get_if<ChangeEditorStage>(&command)})
    {
        state_.stage = change_editor_stage->stage;
    }
    else if (const auto* change_selected_tile{std::get_if<ChangeSelectedTile>(&command)})
    {
        state_.selectedTile = change_selected_tile->new_tile;
        state_.stage = EditorStage::NametableSetValue;
    }
    else if (const auto* change_pattern_table{std::get_if<ChangePatternTable>(&command)})
    {
        state_.selectedPatternTable = change_pattern_table->new_pattern_table;
    }
    else if (const auto* change_tool{std::get_if<ChangeTool>(&command)})
    {
        state_.tool = change_tool->tool;
    }
    else if (const auto* change_collision_visibility{std::get_if<ChangeCollisionVisibility>(&command)})
    {
        state_.showCollisions = change_collision_visibility->visible;
    }
    else if (const auto* save_level{std::get_if<SaveLevel>(&command)})
    {
        Parser::ExportLevelInfo(
            import_export_filepath_,
            level_.nametables
        );
    }
    else if (const auto* load_level{std::get_if<LoadLevel>(&command)})
    {
        Parser::ImportLevelInfo(
            import_export_filepath_,
            level_.nametables
        );
    }
}


//vibe coded layout jumpscare
void LevelEditor::InitializeEditorLayout(
    float window_width,
    float window_height,
    float pattern_table_menu_size
)
{
    const float margin{20.f};

    const float button_width{120.f};
    const float button_height{40.f};

    const float nes_width{256.f};
    const float nes_height{240.f};

    const float available_width{
        window_width - 2.f * margin
    };

    const float available_height{
        window_height - 2.f * margin - button_height
    };

    const float scale{
        std::max(0.f, std::min(
            available_width / (2.f * nes_width),
            available_height / nes_height
        ))
    };

    const float table_width{nes_width * scale};
    const float table_height{nes_height * scale};

    const float totalWidth{2.f * table_width };
    const float startX{(window_width - totalWidth) * 0.5f};

    layout_.nametable_bounds[0] = Rectf{
        startX,
        margin,
        table_width,
        table_height
    };

    layout_.nametable_bounds[1] = Rectf{
        startX + table_width,
        margin,
        table_width,
        table_height
    };

    const float buttonY{margin + table_height };

    layout_.loadButton = Rectf{
        startX,
        buttonY,
        button_width,
        button_height
    };

    layout_.saveButton = Rectf{
        startX + button_width,
        buttonY,
        button_width,
        button_height
    };

    layout_.patternButton = Rectf{
        startX + 2.f * (button_width),
        buttonY,
        button_width,
        button_height
    };

    layout_.patternMenu.left = 50.f;
    layout_.patternMenu.top = 50.f;
    layout_.patternMenu.width = pattern_table_menu_size;
    layout_.patternMenu.height = pattern_table_menu_size;

    layout_.window_width = window_width;
    layout_.window_height = window_height;
}




