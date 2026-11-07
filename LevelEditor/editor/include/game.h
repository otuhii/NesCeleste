#pragma once

#include "engine/pch.h"
#include "defines.h"
#include "Parser.h"
#include <memory>


#pragma region gameInformation

std::string g_WindowTitle{ "NES Level Editor" };

float g_WindowWidth{ 800.f };
float g_WindowHeight{ 800.f };


#pragma endregion gameInformation

#pragma region myStructs

#pragma endregion

#pragma region myVariables

Nametable g_Nametable1{};
Nametable g_Namatable2{};
std::unique_ptr<PatternTables> g_Pattern_tables{};


#pragma endregion

#pragma region gameFunctions
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling


void DrawPatternTable(
    uint8_t pattern_table_index,
    const Point2f& startPos,
    const Point2f& endPos
);

void DrawTile(
    uint8_t pattern_table_index,
    uint8_t tile_index,
    const Point2f& startPos,
    float width,
    float height
);