//
// Created by otuhii on 11/7/26.
//

#include "../include/Parser.h"
#include <fstream>
#include <iostream>

#include "PatternTables.h"

PatternTables Parser::ParseChr(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::binary);

    if (!file)
    {
        std::cerr << "cannot open a file" << std::endl;
        return {};
    }

    return PatternTables{std::vector<uint8_t>{
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>() //because by default it is create with endline
    }};
}
