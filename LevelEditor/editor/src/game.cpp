#include "game.h"

#include <filesystem>
#include <fstream>

#include "userutils.h"
#include <iostream>

using namespace utils;

#pragma region gameFunctions

void Start()
{
	g_editor_layout = CalculateEditorLayout();

	g_Pattern_tables = std::make_unique<PatternTables>(
		Parser::ParseChr("resources/celeste.chr")
	); //expecting 8kb patterns that combines two pattern tables
}

void Draw()
{
	ClearBackground();

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
			picked_pattern_table,
			Point2f{g_editor_layout.patternMenu.left,g_editor_layout.patternMenu.top},
			Point2f{g_editor_layout.patternMenu.left+g_editor_layout.patternMenu.width, g_editor_layout.patternMenu.top+g_editor_layout.patternMenu.height}
		);
	}

	if (g_editor_tool == EditorTool::Collisions || g_show_collisions)
	{
		DrawCollisionOverlay(
			g_name_tables[0],
			g_editor_layout.nametable1
		);

		DrawCollisionOverlay(
			g_name_tables[1],
			g_editor_layout.nametable2
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
	if (key == SDLK_1)
	{
		picked_pattern_table = 0;
	}
	else if (key == SDLK_2)
	{
		picked_pattern_table = 1;
	}
	else if (key == SDLK_t)
	{
		g_editor_tool = EditorTool::Tiles;
		std::cout << "tiles mode" << std::endl;
	}
	else if (key == SDLK_c)
	{
		std::cout << "collision mode" << std::endl;
		g_editor_tool = EditorTool::Collisions;
	}
	else if (key == SDLK_s)
	{
		g_show_collisions = true;
	}
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

		HandleNametableClick(
			g_name_tables[0],
			g_editor_layout.nametable1,
			mouse_pos,
			e.button
		);

		HandleNametableClick(
			g_name_tables[1],
			g_editor_layout.nametable2,
			mouse_pos,
			e.button
		);
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
				tile_height,
				g_background_palettes.palettes[0]
			);
		}
	}

}

void DrawTile(
	uint8_t pattern_table_index,
	uint8_t tile_index,
	const Point2f &startPos,
	float width, float height,
	const Palette& palette
)
{
	const int tile_resolution{8};

	const float tile_pixel_width{width/tile_resolution};
	const float tile_pixel_height{height / tile_resolution};

	const PatternTables::Tile tile {
		g_Pattern_tables->DecodeTile(pattern_table_index, tile_index)
	};

	for (int pixel_row{0}; pixel_row < tile_resolution; ++pixel_row)
	{
		for (int pixel_col{0}; pixel_col < tile_resolution; ++pixel_col)
		{
			const uint8_t pixelIndex{
				tile.pixels[pixel_row * tile_resolution + pixel_col]
			};

			const uint8_t nesColor{
				palette[pixelIndex]
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

	constexpr int palette_regions_per_row{16};

	for (int row{0}; row < tile_count_y; ++row)
	{
		for (int col{0}; col < tile_count_x; ++col)
		{
			const Point2f draw_point{
				nametable_bounds.left + static_cast<float>(col) * tile_width,
				nametable_bounds.top + static_cast<float>(row) * tile_height
			};

			// Which 2×2 region contains this tile?
			const int palette_x{col / 2};
			const int palette_y{row / 2};

			// Select palette index (0–3)
			const uint8_t palette_index{
				nametable.palettes[
					palette_y * palette_regions_per_row + palette_x
				]
			};

			// Get the four actual NES colors
			const Palette& palette{
				g_background_palettes.palettes[palette_index]
			};

			DrawTile(
				picked_pattern_table,
				nametable.tiles[row * tile_count_x + col],
				draw_point,
				tile_width,
				tile_height,
				palette
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

	const Point2f local_pos{		click_pos.x - g_editor_layout.patternMenu.left,
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
	const bool collisionsExported{
		ExportCollisionTables()
	};

	const bool nametablesExported{
		ExportNametables()
	};

	if (collisionsExported && nametablesExported)
	{
		std::cout << "Export successful\n";
	}
	else
	{
		std::cerr << "Export failed\n";
	}
}

bool ExportCollisionTables()
{
	std::filesystem::create_directories(g_export_path);
	std::ofstream file{
		g_export_path+"collisions.bin",
		std::ios::binary
	};

	if (!file)
	{
		std::cerr << "Failed to open collision export file\n";
		return false;
	}

	for (const Nametable& nametable : g_name_tables)
	{
		const auto encoded{
			EncodeCollisionTables(nametable)
		};

		file.write(
			reinterpret_cast<const char*>(encoded.data()),
			encoded.size()
		);

		if (!file)
			return false;
	}

	return true;
}

bool ExportNametables()
{
	std::filesystem::create_directories(g_export_path);


	std::ofstream file{
		g_export_path+"nametables.bin",
		std::ios::binary
	};

	if (!file)
	{
		std::cerr << "Failed to open nametable export file\n";
		return false;
	}

	for (const Nametable& nametable : g_name_tables)
	{
		file.write(
			reinterpret_cast<const char*>(nametable.tiles.data()),
			static_cast<std::streamsize>(nametable.tiles.size())
		);

		if (!file)
			return false;

		const auto attributes{
			EncodeAttributes(nametable)
		};

		file.write(
			reinterpret_cast<const char*>(attributes.data()),
			static_cast<std::streamsize>(attributes.size())
		);

		if (!file)
			return false;
	}

	return true;
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


void CyclePalette(Nametable& nametable, int tileX, int tileY)
{
	const int index{
		(tileY / 2) * 16 + (tileX / 2)
	};

	nametable.palettes[index] =
		(nametable.palettes[index] + 1) & 0x03;
}


void CycleCollision(
	Nametable& nametable,
	int tileX,
	int tileY)
{
	const uint32_t index{
		tileY * tile_count_x + tileX
	};

	const uint8_t current{
		static_cast<uint8_t>(nametable.collisionTypes[index])
	};

	nametable.collisionTypes[index] =
		static_cast<CollisionType>((current+1)&0x03);
}


void HandleNametableClick(
	Nametable& nametable,
	const Rectf& bounds,
	const Point2f& mousePos,
	uint8_t mouseButton)
{
	const auto [x, y]{
		DecodeNametableClickPosition(mousePos, bounds)
	};

	if (x == -1 || y == -1)
		return;

	if (g_editor_tool == EditorTool::Collisions)
	{
		if (mouseButton == SDL_BUTTON_RIGHT)
		{
			CycleCollision(nametable, x, y);
		}

		return;
	}

	// Existing tile editing
	if (mouseButton == SDL_BUTTON_LEFT)
	{
		nametable.tiles[y * tile_count_x + x] =
			static_cast<uint8_t>(picked_tile_index);
	}
	else if (mouseButton == SDL_BUTTON_RIGHT)
	{
		CyclePalette(nametable, x, y);
	}
}


void DrawCollisionOverlay(
	const Nametable& nametable,
	const Rectf& bounds)
{
	const float tileWidth{bounds.width / tile_count_x};
	const float tileHeight{bounds.height / tile_count_y};

	for (int y{0}; y < tile_count_y; ++y)
	{
		for (int x{0}; x < tile_count_x; ++x)
		{
			const auto type{
				nametable.collisionTypes[y * tile_count_x + x]
			};

			switch (type)
			{
				case CollisionType::Empty:
					continue;

				case CollisionType::Solid:
					SetColor(0.f, 0.4f, 1.f, 0.4f);
					break;

				case CollisionType::Spikes:
					SetColor(1.f, 0.f, 0.f, 0.4f);
					break;

				case CollisionType::Special:
					SetColor(0.7f, 0.f, 1.f, 0.4f);
					break;
			}

			FillRect(
				bounds.left + x * tileWidth,
				bounds.top + y * tileHeight,
				tileWidth,
				tileHeight
			);
		}
	}
}

std::array<uint8_t, 240> EncodeCollisionTables(const Nametable& nametable)
{
	std::array<uint8_t, 240> encoded{};

	constexpr int tilesPerByte{4};

	for (int y{0}; y < tile_count_y; ++y)
	{
		for (int x{0}; x < tile_count_x; ++x)
		{
			const uint32_t tileIndex{
				y * tile_count_x + x
			};

			const uint32_t byteIndex{
				y * (tile_count_x / tilesPerByte) + x / tilesPerByte
			};

			const int shift{
				(tilesPerByte - 1 - (x % tilesPerByte)) * 2
			};

			const uint8_t collisionType{
				static_cast<uint8_t>(nametable.collisionTypes[tileIndex])
			};

			encoded[byteIndex] |=
				static_cast<uint8_t>((collisionType & 0x03) << shift);
		}
	}

	return encoded;
}

std::array<uint8_t, 64> EncodeAttributes(const Nametable& nametable)
{
	std::array<uint8_t, 64> encoded{};

	constexpr int attributeWidth{8};
	constexpr int paletteRegionsPerRow{16};
	constexpr int paletteRegionRows{15};

	for (int y{0}; y < 8; ++y)
	{
		for (int x{0}; x < 8; ++x)
		{
			uint8_t attribute{0};

			for (int quadrantY{0}; quadrantY < 2; ++quadrantY)
			{
				for (int quadrantX{0}; quadrantX < 2; ++quadrantX)
				{
					const int paletteX{x * 2 + quadrantX};
					const int paletteY{y * 2 + quadrantY};

					if (paletteY >= paletteRegionRows)
						continue;

					const uint8_t paletteIndex{
						nametable.palettes[
							paletteY * paletteRegionsPerRow + paletteX
						]
					};

					const int shift{
						(quadrantY * 2 + quadrantX) * 2
					};

					attribute |=
						static_cast<uint8_t>((paletteIndex & 0x03) << shift);
				}
			}

			encoded[y * attributeWidth + x] = attribute;
		}
	}

	return encoded;
}

#pragma endregion inputHandling

#pragma region myFunctionDefinitions

#pragma endregion