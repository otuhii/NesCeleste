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
    //0x3F = 00111111
    //so if we and any value uint8_t with 0x3F we would get 00 xx xx xx
    const uint32_t rgb = NES_PALETTE[nesIndex & 0x3F];

    return Color4f
    {
        //we shift our rgb so our desired value occupies lowest 8 bits
        //when we AND that value with 0xFF we just extract all of those lowest 8 bits of uint32_t
        //0xff for uint32_t is 00000000 00000000 00000000 11111111
        static_cast<float>((rgb >> 16) & 0xFF) / 255.0f,
        static_cast<float>((rgb >> 8)  & 0xFF) / 255.0f,
        static_cast<float>(rgb & 0xFF) / 255.0f,
        1.0f
    };
}

#endif //LEVELEDITOR_UTILS_H
