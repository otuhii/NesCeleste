//
// Created by otuhii on 11/21/26.
//

#ifndef LEVELEDITOR_EDITORRENDERER_H
#define LEVELEDITOR_EDITORRENDERER_H

#include "EditorLayout.h"
#include "NESPalette.h"
#include "LevelData.h"
#include "PatternTables.h"
#include "../../engine/include/engine/structs.h"
#include "UserUtils.h"
#include "../../engine/include/engine/utils.h"


class EditorRenderer {
public:
    static void DrawPatternTable(
        uint8_t pattern_table_index,
        const EditorLayout& layout,
        const BackgroundPalettes& palettes,
        const PatternTables& pattern_tables
    );

    static void DrawButtons(
        const EditorLayout& layout
    );

    static void DrawNametable(
        const Nametable &nametable,
        const Rectf &nametable_bounds,
        uint8_t current_pattern_table,
        const BackgroundPalettes& palettes,
        const PatternTables& pattern_tables
    );

    static void DrawCollisionOverlay(
        const Nametable& nametable,
        const Rectf& bounds
    );

private:
    static void DrawTile(
        uint8_t pattern_table_index,
        uint8_t tile_index,
        const Point2f& startPos,
        float width,
        float height,
        const Palette& palette,
        const PatternTables& pattern_tables
    );

};


#endif //LEVELEDITOR_EDITORRENDERER_H
