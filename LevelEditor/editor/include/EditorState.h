//
// Created by otuhii on 11/22/26.
//

#ifndef LEVELEDITOR_EDITORSTATE_H
#define LEVELEDITOR_EDITORSTATE_H
#include <cstdint>

enum class EditorStage{
    NametableSetValue,
    PickPattern
};

enum class EditorTool
{
    Tiles,
    Collisions,
    Eraser
};

struct EditorState
{
    EditorTool tool{EditorTool::Tiles};
    EditorStage stage{EditorStage::NametableSetValue};

    uint8_t selectedTile{0};
    uint8_t selectedPatternTable{0};

    bool showCollisions{false};
};


#endif //LEVELEDITOR_EDITORSTATE_H




