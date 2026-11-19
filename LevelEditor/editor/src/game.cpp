#include "game.h"

#include "userutils.h"
#include <iostream>

using namespace utils;

#pragma region gameFunctions

void Start()
{
	g_editor_layout = CalculateEditorLayout();

	g_Pattern_tables = std::make_unique<PatternTables>(
		Parser::ParseChr("resources/pattern_table.chr")
	); //expecting 8kb patterns that combines two pattern tables
}

void Draw()
{
	ClearBackground();

	uint8_t
		pattern_table_index{0};


	SetColor(1.f, 0.f, 0.f, 1.f);
	FillRect(g_editor_layout.loadButton);
	SetColor(0.f, 1.f, 0.f, 1.f);
	FillRect(g_editor_layout.saveButton);
	SetColor(0.f, 0.f, 1.f, 1.f);
	FillRect(g_editor_layout.patternButton);


	DrawNametable(g_name_tables[0], g_editor_layout.nametable1);
	DrawNametable(g_name_tables[1], g_editor_layout.nametable2);

	if (g_editor_stage == EditorStage::pick_pattern)
	{
		SetColor(0.f, 0.f, 0.f, 0.4f);
		FillRect(0.f, 0.f, g_WindowWidth, g_WindowHeight);

		DrawPatternTable(
			pattern_table_index,
			Point2f{g_editor_layout.patternMenu.left,g_editor_layout.patternMenu.top},
			Point2f{g_editor_layout.patternMenu.left+g_editor_layout.patternMenu.width, g_editor_layout.patternMenu.top+g_editor_layout.patternMenu.height}
		);
	}
}

void Update(float elapsedSec)
{

}

void End()
{

}

#pragma endregion gameFunctions

#pragma region inputHandling

void OnKeyDownEvent(SDL_Keycode key)
{

}

void OnKeyUpEvent(SDL_Keycode key)
{
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{

}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	const Point2f mouse_pos{
		static_cast<float>(e.x), static_cast<float>(e.y)
	};

	if (g_editor_stage == EditorStage::nametable_set_value)
	{
		if (CheckMouseRectCollision(mouse_pos, g_editor_layout.saveButton))
		{
			Export();
		}

		if (CheckMouseRectCollision(mouse_pos, g_editor_layout.loadButton))
		{
			Import();
		}

		if (CheckMouseRectCollision(mouse_pos, g_editor_layout.patternButton))
		{
			g_editor_stage = EditorStage::pick_pattern;
		}

		auto click_indices =
			DecodeNametableClickPosition(mouse_pos, g_editor_layout.nametable1);

		if (click_indices.first != -1 &&
			click_indices.second != -1 &&
			picked_tile_index < 256)
		{
			g_name_tables[0].tiles[click_indices.second * tile_count_x + click_indices.first] = picked_tile_index;
		}

		click_indices =
			DecodeNametableClickPosition(mouse_pos, g_editor_layout.nametable2);
		if (click_indices.first != -1 &&
			click_indices.second != -1 &&
			picked_tile_index < 256)
		{
			g_name_tables[1].tiles[click_indices.second * tile_count_x + click_indices.first] = picked_tile_index;
		}
	}
	else if (g_editor_stage == EditorStage::pick_pattern)
	{
		if (!CheckMouseRectCollision(mouse_pos, g_editor_layout.patternMenu))
		{
			g_editor_stage = EditorStage::nametable_set_value;
			return;
		}
		auto coords =
			DecodePatterntableClickPosition(mouse_pos);

		if (coords.first != -1 &&
			coords.second != -1)
		{
			//have a variable for 16
			picked_tile_index = coords.second * 16 + coords.first;
			g_editor_stage = EditorStage::nametable_set_value;
		}
	}
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}



//vibe coded layout jumpscare
EditorLayout CalculateEditorLayout()
{
	EditorLayout layout{};

	const float margin{20.f};

	const float buttonWidth{120.f};
	const float buttonHeight{40.f};

	const float nesWidth{256.f};
	const float nesHeight{240.f};

	const float windowWidth{
		g_WindowWidth
	};

	const float windowHeight{
		g_WindowHeight
	};

	const float availableWidth{
		windowWidth - 2.f * margin
	};

	const float availableHeight{
		windowHeight - 2.f * margin - buttonHeight
	};

	const float scale{
		std::max(0.f, std::min(
			availableWidth / (2.f * nesWidth),
			availableHeight / nesHeight
		))
	};

	const float tableWidth{nesWidth * scale};
	const float tableHeight{nesHeight * scale};

	const float totalWidth{2.f * tableWidth };
	const float startX{(windowWidth - totalWidth) * 0.5f};

	layout.nametable1 = Rectf{
		startX,
		margin,
		tableWidth,
		tableHeight
	};

	layout.nametable2 = Rectf{
		startX + tableWidth,
		margin,
		tableWidth,
		tableHeight
	};

	const float buttonY{margin + tableHeight };

	layout.loadButton = Rectf{
		startX,
		buttonY,
		buttonWidth,
		buttonHeight
	};

	layout.saveButton = Rectf{
		startX + buttonWidth,
		buttonY,
		buttonWidth,
		buttonHeight
	};

	layout.patternButton = Rectf{
		startX + 2.f * (buttonWidth),
		buttonY,
		buttonWidth,
		buttonHeight
	};

	layout.patternMenu.left = 50.f;
	layout.patternMenu.top = 50.f;
	layout.patternMenu.width = g_pattern_table_menu_size;
	layout.patternMenu.height = g_pattern_table_menu_size;

	return layout;
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
				startPos.x + static_cast<float>(tile_col) * tile_width,
				startPos.y + static_cast<float>(tile_row) * tile_height
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

bool CheckMouseRectCollision(const Point2f &mouse_pos, const Rectf& bounding_box)
{
	if (mouse_pos.x >= bounding_box.left &&
		mouse_pos.x <= bounding_box.left+bounding_box.width &&
		mouse_pos.y >= bounding_box.top &&
		mouse_pos.y <= bounding_box.top+bounding_box.height)
	{
		return true;
	}

	return false;
}



void DrawNametable(const Nametable &nametable, const Rectf& nametable_bounds)
{
	const float
		tile_width{nametable_bounds.width / tile_count_x};
	const float
		tile_height{nametable_bounds.height / tile_count_y};

	for (int row{0}; row < tile_count_y; ++row)
	{
		for (int col{0}; col < tile_count_x; ++col)
		{
			const Point2f draw_point{
				nametable_bounds.left + static_cast<float>(col) * tile_width,
				nametable_bounds.top + static_cast<float>(row) * tile_height
			};

			DrawTile(
				picked_pattern_table,
				nametable.tiles[row * tile_count_x + col],
				draw_point,
				tile_width,
				tile_height
			);
		}
	}
}

std::pair<int, int> DecodeNametableClickPosition(
	const Point2f &click_pos,
	const Rectf& nametable_bounds
)
{
	if (!CheckMouseRectCollision(click_pos, nametable_bounds))
	{
		return std::make_pair(-1, -1);
	}

	const Point2f local_pos{
		click_pos.x - nametable_bounds.left,
		click_pos.y - nametable_bounds.top
	};

	const float
		tile_width{nametable_bounds.width / tile_count_x};
	const float
		tile_height{nametable_bounds.height / tile_count_y};

	return std::make_pair(
		static_cast<int>(std::floorf(local_pos.x/tile_width)),
		static_cast<int>(std::floorf(local_pos.y/tile_height))
	);
}

std::pair<int, int> DecodePatterntableClickPosition(
	const Point2f &click_pos
)
{
	if (!CheckMouseRectCollision(click_pos, g_editor_layout.patternMenu))
	{
		return std::make_pair(-1, -1);
	}

	const Point2f local_pos{
		click_pos.x - g_editor_layout.patternMenu.left,
		click_pos.y - g_editor_layout.patternMenu.top
	};

	const float
		tile_dimensions{g_editor_layout.patternMenu.width / 16};

	return std::make_pair(
		static_cast<int>(std::floorf(local_pos.x/tile_dimensions)),
		static_cast<int>(std::floorf(local_pos.y/tile_dimensions))
	);

}

void Export()
{
	ExportCollisionTables();
	ExportNametables();
}

bool ExportCollisionTables()
{
	return false;
}

bool ExportNametables()
{
	return false;
}

void Import()
{
	ImportCollisionTables();
	ImportNametables();
}

bool ImportCollisionTables()
{
	return false;
}

bool ImportNametables()
{
	return false;
}

#pragma endregion inputHandling

#pragma region myFunctionDefinitions

#pragma endregion