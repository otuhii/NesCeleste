#pragma once

#include "engine/pch.h"
#include "defines.h"
#include "Parser.h"
#include <memory>


#pragma region gameInformation

std::string g_WindowTitle{ "NES Level Editor" };

float g_WindowWidth{ 2000.f };
float g_WindowHeight{ 1000.f };


#pragma endregion gameInformation

#pragma region myStructs
enum class EditorStage{
    nametable_set_value,
    pick_pattern
};

struct EditorLayout
{
    Rectf nametable1{};
    Rectf nametable2{};

    Rectf loadButton{};
    Rectf saveButton{};
    Rectf patternButton{};
};


#pragma endregion

#pragma region myVariables

EditorLayout
    g_editor_layout{};

std::array<Nametable, 2>
    g_name_tables{};

std::unique_ptr<PatternTables>
    g_Pattern_tables{};

EditorStage
    g_editor_stage{EditorStage::nametable_set_value};

const float
    g_pattern_table_menu_size{800.f};

const std::string
    g_export_path{};

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

EditorLayout
    CalculateEditorLayout();

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

