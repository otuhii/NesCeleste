//
// Created by otuhii on 11/21/26.
//

#ifndef LEVELEDITOR_EDITORCOMMAND_H
#define LEVELEDITOR_EDITORCOMMAND_H

#include <variant>
#include "EditorState.h"

struct ChangeTile {
    std::size_t nametable_index{};
    int x{};
    int y{};
    uint8_t tileIndex;
};

struct CyclePalette {
    std::size_t nametable_index{};
    int x{};
    int y{};
};

struct CycleCollision {
    std::size_t nametable_index{};
    int x{};
    int y{};
};

// struct ChangePickedPattern {
//     int x{};
//     int y{};
// };

struct ChangeEditorStage {
    EditorStage stage{};
};

struct ChangeSelectedTile {
    int new_tile{};
};

struct ChangePatternTable {
    int new_pattern_table{};
};

struct ChangeTool {
    EditorTool tool{};
};

struct ChangeCollisionVisibility {
    bool visible{};
};

struct SaveLevel{};
struct LoadLevel{};


using EditorCommand = std::variant<
    ChangeTile,
    CyclePalette,
    CycleCollision,
    SaveLevel,
    LoadLevel,
    ChangeEditorStage,
    ChangeSelectedTile,
    ChangePatternTable,
    ChangeTool,
    ChangeCollisionVisibility
>;

#endif //LEVELEDITOR_EDITORCOMMAND_H
