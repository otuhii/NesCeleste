//
// Created by otuhii on 11/7/26.
//

#ifndef LEVELEDITOR_PARSER_H
#define LEVELEDITOR_PARSER_H
#include <string>
#include <vector>
#include <cinttypes>

#include "PatternTables.h"

class Parser {
public:
    static PatternTables ParseChr(const std::string& filepath);
};


#endif //LEVELEDITOR_PARSER_H
