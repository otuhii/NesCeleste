//
// Created by otuhii on 11/7/26.
//

#ifndef LEVELEDITOR_UTILS_H
#define LEVELEDITOR_UTILS_H
#include "engine/structs.h"

namespace UserUtils {
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

    inline void CyclePalette(
        Nametable& nametable,
        int tileX,
        int tileY
    )
    {
        const int index{
            (tileY / 2) * 16 + (tileX / 2)
        };

        nametable.palettes[index] =
            (nametable.palettes[index] + 1) & 0x03;
    }

    inline void CycleCollision(
        Nametable& nametable,
        int tileX,
        int tileY
    )
    {
        const uint32_t index{
            tileY * Nametable::tile_count_x_ + tileX
        };

        const uint8_t current{
            static_cast<uint8_t>(nametable.collisionTypes[index])
        };

        nametable.collisionTypes[index] =
            static_cast<CollisionType>((current+1)&0x03);
    }
}

#endif //LEVELEDITOR_UTILS_H
