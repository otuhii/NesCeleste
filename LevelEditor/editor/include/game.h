#pragma once

#include "engine/pch.h"
#include "defines.h"
#include "Parser.h"
#include <memory>


#pragma region gameInformation

std::string g_WindowTitle{ "NES Level Editor" };

float g_WindowWidth{ 2800.f };
float g_WindowHeight{ 1300.f };


#pragma endregion gameInformation

#pragma region myStructs
enum class EditorStage{
    nametable_set_value,
    pick_pattern
};

enum class EditorTool
{
    Tiles,
    Collisions
};

struct EditorLayout
{
    Rectf nametable1{};
    Rectf nametable2{};

    Rectf loadButton{};
    Rectf saveButton{};
    Rectf patternButton{};

    Rectf patternMenu{};
};


#pragma endregion

#pragma region myVariables

constexpr BackgroundPalettes g_background_palettes{
    std::array<Palette, 4>{
        Palette{0x2F, 0x30, 0x31, 0x21},
        Palette{0x0F, 0x19, 0x29, 0x39},
        Palette{0x0F, 0x11, 0x21, 0x31},
        Palette{0x0F, 0x06, 0x17, 0x28}
    }
};

EditorTool g_editor_tool{EditorTool::Tiles};

constexpr uint32_t
    tile_count_x{32};
constexpr uint32_t
    tile_count_y{30};

bool
    g_show_collisions{};

int
    picked_pattern_table{};
int
    picked_tile_index{3};

EditorLayout
    g_editor_layout{};

std::array<Nametable, 2>
    g_name_tables{};

std::unique_ptr<PatternTables>
    g_Pattern_tables{};

EditorStage
    g_editor_stage{EditorStage::nametable_set_value};

const float
    g_pattern_table_menu_size{1000.f};

const std::string
    g_export_path{"../../src/visual_data/"};

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
    float height,
    const Palette& palette
);

bool CheckMouseRectCollision(
    const Point2f &mouse_pos,
    const Rectf& bounding_box
);

void DrawNametable(
    const Nametable& nametable,
    const Rectf& nametable_bounds
);

std::pair<int, int> DecodeNametableClickPosition(
    const Point2f& click_pos,
    const Rectf& nametable_bounds
);

std::pair<int, int> DecodePatterntableClickPosition(
    const Point2f& click_pos
);

void CyclePalette(
    Nametable& nametable,
    int tileX, int tileY
);

void Export();
bool ExportCollisionTables();
bool ExportNametables();

void Import();
bool ImportCollisionTables();
bool ImportNametables();

void CycleCollision(
    Nametable& nametable,
    int tileX,
    int tileY
);

void HandleNametableClick(
    Nametable& nametable,
    const Rectf& bounds,
    const Point2f& mousePos,
    uint8_t mouseButton
);

void DrawCollisionOverlay(
    const Nametable& nametable,
    const Rectf& bounds
);

std::array<uint8_t, 240> EncodeCollisionTables(const Nametable& nametable);

std::array<uint8_t, 64> EncodeAttributes(const Nametable& nametable);
