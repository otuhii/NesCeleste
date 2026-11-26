#pragma once
#include "LevelEditor.h"


inline std::string      g_WindowTitle{ "NES Level Editor" };
inline float            g_WindowWidth{ 2800.f };
inline float            g_WindowHeight{ 1300.f };
inline float constexpr  g_Pattern_table_menu_size{1000.f};

inline LevelEditor g_Level_editor{};

void Start();
void Draw();
void Update(float elapsedSec);
void End();

void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);


