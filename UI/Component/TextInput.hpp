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
        float FontShiftRatio;
        bool RoundCorner;
        ALLEGRO_COLOR BackgroundColor;
        ALLEGRO_COLOR BorderColor;
        ALLEGRO_COLOR InvalidColor;
        ALLEGRO_COLOR EditingColor;
        bool Enable;
        bool Visible;
        bool Editing;
        bool InvalidDisplay;
        bool Used;
        bool AllowWhiteSpace;
    public:
        explicit TextInput(
            const std::string& data, const std::string& font, int maxLength,
            float x, float y, float width, float height, float fontShiftRatio,
            bool allowWhiteSpace = false, bool roundCorner = true, float margin = 0, float borderWidth = 10, float anchorX = 0, float anchorY = 0,
            ALLEGRO_COLOR textColor       = {0, 0, 0, 1},
            ALLEGRO_COLOR backgroundColor = {1, 1, 1, 1},
            ALLEGRO_COLOR borderColor     = {0, 1, 0, 1},
            ALLEGRO_COLOR invalidColor    = {1, .5, .5, 1},
            ALLEGRO_COLOR editingColor    = {.4, .6, 1, 1}
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
        void Focus(bool state = true);
        bool IsInvalidDisplay() const;
        bool IsInvalidContent() const;
        bool ConsumeSlot();
        bool IsConsumedSlot() const;
        void ToggleEnable(bool status = true);
        void ToggleVisible(bool status = true);
    };
}

#endif