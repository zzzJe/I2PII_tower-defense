#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP

#include <allegro5/allegro_font.h>
#include <allegro5/color.h>
#include <string>
#include <memory>

#include "Label.hpp"
#include "Engine/IControl.hpp"

namespace Engine {
    class TextInput : public Label, public IControl {
    private:
        int MaxLength;
        float Width;
        float Height;
        float Margin;
        float BorderWidth;
        bool RoundCorner;
        ALLEGRO_COLOR BackgroundColor;
        ALLEGRO_COLOR BorderColor;
        ALLEGRO_COLOR InvalidColor;
        ALLEGRO_COLOR EditingColor;
        bool Editing;
        bool InvalidDisplay;
        bool Used;
    public:
        explicit TextInput(
            const std::string& data, const std::string& font, int maxLength,
            float x, float y, float width, float height,
            bool roundCorner = true, float margin = 0, float borderWidth = 10, float anchorX = 0, float anchorY = 0,
            ALLEGRO_COLOR textColor = (ALLEGRO_COLOR){.r = 0, .g = 0, .b = 0, .a = 1},
            ALLEGRO_COLOR backgroundColor = (ALLEGRO_COLOR){.r = 1, .g = 1, .b = 1, .a = 1},
            ALLEGRO_COLOR borderColor = (ALLEGRO_COLOR){.r = 0, .g = 1, .b = 0, .a = 1},
            ALLEGRO_COLOR invalidColor = (ALLEGRO_COLOR){.r = 1, .g = .5, .b = .5, .a = 1},
            ALLEGRO_COLOR editingColor = (ALLEGRO_COLOR){.r = .4, .g = .6, .b = 1, .a = 1}
        );
        float GetFullWidth();
        float GetFullHeight();
        float GetWidth();
        float GetHeight();
        void Draw() const override;
        void Update(int keycode);
        void Update(char charactor);
        void Update(std::string text);
        void Backspace(std::size_t length = 1);
        void Clear();
        void Replace(std::string text);
        void OnMouseDown(int button, int mx, int my) override;
        void OnKeyDown(int keyCode) override;
        bool IsInvalidDisplay() const;
        bool IsInvalidContent() const;
        bool ConsumeSlot();
        bool IsConsumedSlot() const;
    };
}

#endif