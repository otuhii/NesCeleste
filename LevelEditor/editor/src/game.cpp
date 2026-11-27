#include "game.h"

void Start()
{
	g_Level_editor.Start(
		"resources/celeste.chr",
		g_WindowWidth,
		g_WindowHeight,
		g_Pattern_table_menu_size,
		"resources/level_info/"
	);
}

void Draw()
{
	utils::ClearBackground(Color4f{0.f, 0.f, 0.f, 1.f});

	g_Level_editor.Draw();
}

void Update(float elapsedSec)
{

}

void End()
{

}


#pragma region inputHandling

void OnKeyDownEvent(SDL_Keycode key)
{
}

void OnKeyUpEvent(SDL_Keycode key)
{
	g_Level_editor.OnKeyUp(key);
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{

}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	g_Level_editor.OnMouseDown(e);
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}


#pragma endregion inputHandling
