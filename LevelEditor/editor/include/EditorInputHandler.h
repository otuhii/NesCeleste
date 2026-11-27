//
// Created by otuhii on 11/21/26.
//

#ifndef LEVELEDITOR_EDITORINPUTHANDLER_H
#define LEVELEDITOR_EDITORINPUTHANDLER_H
#include <SDL.h>
#include "../../engine/include/engine/structs.h"
#include "EditorLayout.h"
#include "EditorCommand.h"
#include "PatternTables.h"
#include <optional>

class EditorInputHandler {
public:
    static std::optional<EditorCommand> OnKeyUp(
        const EditorState& state,
        SDL_Keycode key
    );
    static std::optional<EditorCommand> OnMouseDown(
        const EditorState& state,
        const EditorLayout& layout,
        const SDL_MouseButtonEvent& event
    );
private:
    static std::optional<EditorCommand>  HandleNametableClick(
        const EditorState& state,
        const EditorLayout& layout,
        const SDL_MouseButtonEvent& event
    );

    static std::optional<EditorCommand> HandleButtonClick(
        const EditorLayout& layout,
        const SDL_MouseButtonEvent& event
    );

    static std::optional<EditorCommand> HandlePatternTableClick(
        const EditorState& state,
        const EditorLayout& layout,
        const SDL_MouseButtonEvent& event
    );

    static bool CheckMouseRectCollision(
        const Point2f &mouse_pos,
        const Rectf& bounding_box
    );

    static std::pair<int, int> DecodeNametableClickPosition(
        const Point2f& click_pos,
        const Rectf& nametable_bounds
    );

    static std::pair<int, int> DecodePatternTableClickPosition(
        const Point2f& click_pos,
        const EditorLayout& layout
    );
};


#endif //LEVELEDITOR_EDITORINPUTHANDLER_H
