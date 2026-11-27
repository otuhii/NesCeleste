//
// Created by otuhii on 11/21/26.
//

#ifndef LEVELEDITOR_LEVELEDITOR_H
#define LEVELEDITOR_LEVELEDITOR_H
#include "EditorInputHandler.h"
#include "PatternTables.h"
#include "EditorRenderer.h"

class LevelEditor {
public:
    LevelEditor() = default;

    void Start(
        const std::string& pattern_table_filepath,
        float window_width,
        float window_height,
        float pattern_table_menu_size,
        const std::string& import_export_path
    );

    void Draw() const;
    void OnMouseDown(
        const SDL_MouseButtonEvent& event
    );
    void OnKeyUp(
        SDL_Keycode key
    );

private:
    Level level_{};
    EditorState state_{};
    EditorLayout layout_{};

    PatternTables pattern_tables_{};

    std::string import_export_filepath_{};

    void ExecuteCommand(
        const EditorCommand& command
    );

    void InitializeEditorLayout(
        float windowWidth,
        float windowHeight,
        float pattern_table_menu_size
    );



    static constexpr BackgroundPalettes background_palettes_{
        std::array<Palette, 4>{
            Palette{0x2F, 0x30, 0x21, 0x11},
            Palette{0x0F, 0x30, 0x3d, 0x2d},
            Palette{0x0f, 0x39, 0x29, 0x19},
            Palette{0x0f, 0x37, 0x16, 0x06}
        }
    };
};


#endif //LEVELEDITOR_LEVELEDITOR_H
