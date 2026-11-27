//
// Created by otuhii on 11/7/26.
//

#ifndef LEVELEDITOR_PARSER_H
#define LEVELEDITOR_PARSER_H
#include <string>
#include <vector>
#include <cinttypes>

#include "PatternTables.h"
#include "LevelData.h"

class Parser {
public:
    static PatternTables ParseChr(const std::string& filepath);

    static void ExportLevelInfo(
        const std::string& filepath,
        const std::array<Nametable, 2>& nametables
    );
    static void ImportLevelInfo(
        const std::string& filepath,
        std::array<Nametable, 2>& nametables
    );

private:
    static bool ExportCollisionTables(
        const std::string& filepath,
        const std::array<Nametable, 2>& nametables
    );
    static bool ExportNametables(
        const std::string& filepath,
        const std::array<Nametable, 2>& nametables
    );

    static bool ImportCollisionTables(
        const std::string& filepath,
        std::array<Nametable, 2>& nametables
    );
    static bool ImportNametables(
        const std::string& filepath,
        std::array<Nametable, 2>& nametables
    );

    static std::array<uint8_t, 240>
        EncodeCollisionTables(const Nametable& nametable);
    static std::array<uint8_t, 64>
        EncodeAttributes(const Nametable& nametable);

};


#endif //LEVELEDITOR_PARSER_H
