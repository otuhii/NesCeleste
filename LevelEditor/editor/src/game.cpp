#include "game.h"

#include "userutils.h"

using namespace utils;

#pragma region gameFunctions

void Start()
{
	g_Pattern_tables = std::make_unique<PatternTables>(
		Parser::ParseChr("resources/pattern_table.chr")
	); //expecting 8kb patterns that combines two pattern tables
}

void Draw()
{
	ClearBackground();

	uint8_t
		pattern_table_index{0};

	DrawPatternTable(
		pattern_table_index,
		Point2f{0.f, 0.f},
		Point2f{400.f, 400.f}
	);
}

void Update(float elapsedSec)
{

}

void End()
{

}

#pragma endregion gameFunctions

#pragma region inputHandling

// Called once when a key is pressed.
void OnKeyDownEvent(SDL_Keycode key)
{
}

// Called once when a key is released.
void OnKeyUpEvent(SDL_Keycode key)
{
}

// Called whenever the mouse moves.
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
}

// Called once when a mouse button is pressed.
void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

// Called once when a mouse button is released.
void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void DrawPatternTable(uint8_t pattern_table_index, const Point2f &startPos, const Point2f &endPos)
{
	const int y_tile_count{16};
	const int x_tile_count{16};


	const float width{endPos.x - startPos.x};
	const float height{endPos.y-startPos.y};

	const float tile_width{width/x_tile_count};
	const float tile_height{height/y_tile_count};

	for (int tile_row{}; tile_row < y_tile_count; ++tile_row)
	{
		for (int tile_col{}; tile_col < x_tile_count; ++tile_col)
		{
			const uint8_t tileIndex{
				static_cast<uint8_t>(tile_row * x_tile_count + tile_col)
			};

			const Point2f tilePos{
				startPos.x + tile_col * tile_width,
				startPos.y + tile_row * tile_height
			};

			DrawTile(
				pattern_table_index,
				tileIndex,
				tilePos,
				tile_width,
				tile_height
			);
		}
	}

}

void DrawTile(uint8_t pattern_table_index, uint8_t tile_index, const Point2f &startPos, float width, float height)
{
	const int tile_resolution{8};

	const float tile_pixel_width{width/tile_resolution};
	const float tile_pixel_height{height / tile_resolution};

	const PatternTables::Tile tile {
		g_Pattern_tables->DecodeTile(pattern_table_index, tile_index)
	};

	//temp 4 NES color indices
	constexpr std::array<uint8_t, 4> previewPalette{
		0x0F, 0x16, 0x27, 0x30
	};

	for (int pixel_row{0}; pixel_row < tile_resolution; ++pixel_row)
	{
		for (int pixel_col{0}; pixel_col < tile_resolution; ++pixel_col)
		{
			const uint8_t pixelIndex{
				tile.pixels[pixel_row * tile_resolution + pixel_col]
			};

			const uint8_t nesColor{
				previewPalette[pixelIndex]
			};

			const Color4f color{
				GetNesColor(nesColor)
			};

			utils::SetColor(color);

			Point2f position{
				startPos.x + static_cast<float>(pixel_col) * tile_pixel_width,
				startPos.y + static_cast<float>(pixel_row) * tile_pixel_height
			};

			FillRect(
				position,
				tile_pixel_width,
				tile_pixel_height
			);
		}
	}

}

#pragma endregion inputHandling

#pragma region myFunctionDefinitions

#pragma endregion