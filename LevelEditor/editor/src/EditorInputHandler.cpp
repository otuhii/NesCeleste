//
// Created by otuhii on 11/21/26.
//

#include "EditorInputHandler.h"

#include "LevelData.h"


std::optional<EditorCommand> EditorInputHandler::OnKeyUp(
    const EditorState &state,
    SDL_Keycode key
)
{
    switch (key)
    {
        case SDLK_1:
        {
            return EditorCommand{
                ChangePatternTable{0}
            };
            break;
        }
        case SDLK_2:
        {
            return EditorCommand{
                ChangePatternTable{1}
            };
            break;
        }
        case SDLK_t:
        {
            return EditorCommand{
                ChangeTool{EditorTool::Tiles}
            };
            break;
        }
        case SDLK_c:
        {
            return EditorCommand{
                ChangeTool{EditorTool::Collisions}
            };
            break;
        }
        case SDLK_s:
        {
            return EditorCommand{
                ChangeCollisionVisibility{!state.showCollisions}
            };
            break;
        }
        case SDLK_DELETE:
        {
            return EditorCommand{
                ClearAllNametables{}
            };
            break;
        }
        case SDLK_e:
        {
            return EditorCommand{
                ChangeTool{EditorTool::Eraser}
            };
        }
        default:
            return std::nullopt;
            break;
    }
}

std::optional<EditorCommand> EditorInputHandler::OnMouseDown(
    const EditorState &state,
    const EditorLayout &layout,
    const SDL_MouseButtonEvent &event
)
{
    if (state.stage == EditorStage::NametableSetValue)
    {
        const auto command{
            HandleButtonClick(layout, event)
        };
        if (!command.has_value())
        {
            return HandleNametableClick(state, layout, event);
        }
        return command;
    }

    if (state.stage == EditorStage::PickPattern)
    {
        return HandlePatternTableClick(state, layout, event);
    }

    return std::nullopt;
}

std::optional<EditorCommand> EditorInputHandler::HandleNametableClick(
    const EditorState& state,
    const EditorLayout& layout,
    const SDL_MouseButtonEvent& event
)
{
    const Point2f mouse_pos{
        static_cast<float>(event.x),
        static_cast<float>(event.y)
    };

    for (std::size_t nametable_index{}; nametable_index < layout.nametable_bounds.size(); ++nametable_index)
    {
        const Rectf& current_nametable_bounds{
            layout.nametable_bounds[nametable_index]
        };

        if (!CheckMouseRectCollision(mouse_pos, current_nametable_bounds))
        {
            continue;
        }

        const auto [x, y] {
            DecodeNametableClickPosition(mouse_pos, current_nametable_bounds)
        };

        if (x == -1 || y == -1)
        {
            return std::nullopt;
        }

        if (state.tool == EditorTool::Collisions)
        {
            if (event.button == SDL_BUTTON_LEFT)
            {
                return EditorCommand{
                    CycleCollision{nametable_index, x, y}
                };
            }
            return std::nullopt;
        }

        if (state.tool == EditorTool::Eraser)
        {
            if (event.button == SDL_BUTTON_LEFT)
            {
                return EditorCommand{
                    ChangeTile{nametable_index, x, y, Nametable::defalt_nametable_index_}
                };
            }
        }

        if (event.button == SDL_BUTTON_LEFT)
        {
            return EditorCommand{
                ChangeTile{nametable_index, x, y, state.selectedTile}
            };
        }

        if (event.button == SDL_BUTTON_RIGHT)
        {
            return EditorCommand{
                CyclePalette{nametable_index, x, y}
            };
        }

        return std::nullopt;
    }

    return std::nullopt;

}

std::optional<EditorCommand> EditorInputHandler::HandleButtonClick(
    const EditorLayout &layout,
    const SDL_MouseButtonEvent &event
)
{
    const Point2f mouse_pos{
        static_cast<float>(event.x),
        static_cast<float>(event.y)
    };

    if (CheckMouseRectCollision(mouse_pos, layout.saveButton))
    {
        return EditorCommand{
            SaveLevel{}
        };
    }

    if (CheckMouseRectCollision(mouse_pos, layout.loadButton))
    {
        return EditorCommand{
            LoadLevel{}
        };
    }

    if (CheckMouseRectCollision(mouse_pos, layout.patternButton))
    {
        return EditorCommand{
            ChangeEditorStage{EditorStage::PickPattern}
        };
    }

    return std::nullopt;
}

std::optional<EditorCommand> EditorInputHandler::HandlePatternTableClick(
    const EditorState& state,
    const EditorLayout &layout,
    const SDL_MouseButtonEvent &event
)
{
    const Point2f mouse_pos{
        static_cast<float>(event.x),
        static_cast<float>(event.y)
    };

    if (!CheckMouseRectCollision(mouse_pos, layout.patternMenu))
    {
        return EditorCommand{
            ChangeEditorStage{EditorStage::NametableSetValue}
        };
    }

    const auto [x, y] {
        DecodePatternTableClickPosition(mouse_pos, layout)
    };
    if (x == -1 || y == -1)
    {
        return std::nullopt;
    }

    return EditorCommand{
        ChangeSelectedTile{y * PatternTables::tile_count_ + x}
    };
}

bool EditorInputHandler::CheckMouseRectCollision(
    const Point2f &mouse_pos,
    const Rectf &bounding_box
)
{
    if (mouse_pos.x > bounding_box.left &&
            mouse_pos.x < bounding_box.left+bounding_box.width &&
            mouse_pos.y > bounding_box.top &&
            mouse_pos.y < bounding_box.top+bounding_box.height)
    {
        return true;
    }

    return false;
}

std::pair<int, int> EditorInputHandler::DecodeNametableClickPosition(
    const Point2f &click_pos,
    const Rectf &nametable_bounds
)
{
    if (!CheckMouseRectCollision(click_pos, nametable_bounds))
    {
        return std::make_pair(-1, -1);
    }

    const Point2f local_pos{
        click_pos.x - nametable_bounds.left,
        click_pos.y - nametable_bounds.top
    };

    const float
        tile_width{nametable_bounds.width / Nametable::tile_count_x_};
    const float
        tile_height{nametable_bounds.height / Nametable::tile_count_y_};

    return std::make_pair(
        static_cast<int>(std::floorf(local_pos.x/tile_width)),
        static_cast<int>(std::floorf(local_pos.y/tile_height))
    );
}

std::pair<int, int> EditorInputHandler::DecodePatternTableClickPosition(
    const Point2f &click_pos,
    const EditorLayout& layout
)
{
    if (!CheckMouseRectCollision(click_pos, layout.patternMenu))
    {
        return std::make_pair(-1, -1);
    }

    const Point2f local_pos{
        click_pos.x - layout.patternMenu.left,
        click_pos.y - layout.patternMenu.top
    };

    const float
        tile_dimensions{layout.patternMenu.width / 16};

    return std::make_pair(
        static_cast<int>(std::floorf(local_pos.x/tile_dimensions)),
        static_cast<int>(std::floorf(local_pos.y/tile_dimensions))
    );
}
