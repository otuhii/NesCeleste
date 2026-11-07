//
// Created by otuhii on 11/7/26.
//

#ifndef LEVELEDITOR_UTILS_H
#define LEVELEDITOR_UTILS_H
#include <cinttypes>
#include "defines.h"
#include "engine/structs.h"

inline Color4f GetNesColor(uint8_t nesIndex)
{
    const uint32_t rgb = NES_PALETTE[nesIndex & 0x3F];

    return Color4f
    {
        static_cast<float>((rgb >> 16) & 0xFF) / 255.0f,
        static_cast<float>((rgb >> 8)  & 0xFF) / 255.0f,
        static_cast<float>(rgb & 0xFF) / 255.0f,
        1.0f
    };
}

#endif //LEVELEDITOR_UTILS_H
