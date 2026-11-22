//
// Created by otuhii on 11/22/26.
//

#ifndef LEVELEDITOR_EDITORLAYOUT_H
#define LEVELEDITOR_EDITORLAYOUT_H
#include <array>
#include "../include/engine/structs.h"

struct EditorLayout
{
    std::array<Rectf, 2> nametable_bounds{};

    Rectf loadButton{};
    Rectf saveButton{};
    Rectf patternButton{};

    Rectf patternMenu{};

    float window_width{};
    float window_height{};
};

#endif //LEVELEDITOR_EDITORLAYOUT_H


