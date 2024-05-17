#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include "TextInput.hpp"

Engine::TextInput::TextInput(
    const std::string& data, const std::string& font,
    float x, float y, float width, float height,
    bool roundCorner, float margin, float borderWidth, float anchorX, float anchorY,
    ALLEGRO_COLOR textColor,
    ALLEGRO_COLOR backgroundColor,
    ALLEGRO_COLOR borderColor
):
    Label(data, font, height * 0.9, x, y, textColor.r, textColor.g, textColor.b, textColor.a, anchorX, anchorY),
    Width(width),
    Height(height),
    Margin(margin),
    BorderWidth(borderWidth),
    RoundCorner(roundCorner),
    BackgroundColor(backgroundColor),
    BorderColor(borderColor)
{}

float Engine::TextInput::GetFullWidth() {
    return this->BorderWidth * 2 + this->Margin * 2 + this->Width;
}

float Engine::TextInput::GetFullHeight() {
    return this->BorderWidth * 2 + this->Margin * 2 + this->Height;
}

float Engine::TextInput::GetWidth() {
    return this->Margin * 2 + this->Width;
}

float Engine::TextInput::GetHeight() {
    return this->Margin * 2 + this->Height;
}


void Engine::TextInput::Draw() const {
    al_draw_filled_rectangle(
        this->Position.x + this->BorderWidth - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
        this->Position.y + this->BorderWidth - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
        this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
        this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
        this->BackgroundColor
    );
    if (this->RoundCorner) {
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderColor,
            this->BorderWidth
        );
        al_draw_filled_circle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            this->BorderColor
        );
        al_draw_filled_circle(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            this->BorderColor
        );
        al_draw_filled_circle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            this->BorderColor
        );
        al_draw_filled_circle(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            this->BorderColor
        );
    } else {
        al_draw_rectangle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderColor,
            this->BorderWidth
        );
    }
    if (this->GetTextWidth() > this->Width) {
        std::string to_display_text;
        for (std::size_t shift = 0; shift < this->Text.length(); shift++) {
            to_display_text = this->Text.substr(shift);
            if (al_get_text_width(this->font.get(), to_display_text.c_str()) > this->Width)
                continue;
            al_draw_text(
                this->font.get(),
                this->Color,
                this->Position.x + this->BorderWidth + this->Margin + this->Width - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
                this->Position.y + this->BorderWidth + this->Margin - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
                ALLEGRO_ALIGN_RIGHT,
                to_display_text.c_str()
            );
            break;
        }
    } else {
        al_draw_text(
            this->font.get(),
            this->Color,
            this->Position.x + this->BorderWidth + this->Margin - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth + this->Margin - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            ALLEGRO_ALIGN_LEFT,
            this->Text.c_str()
        );
    }
}

void Engine::TextInput::Update(int keycode) {
    if (keycode == ALLEGRO_KEY_BACKSPACE)
        return this->Backspace();
    auto is_valid_inputcode = [] (int keycode) {
        return keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_PAD_9
            || keycode == ALLEGRO_KEY_MINUS
            || keycode == ALLEGRO_KEY_PAD_MINUS;
    };
    if (!is_valid_inputcode(keycode))
        return;
    ALLEGRO_KEYBOARD_STATE state;
    al_get_keyboard_state(&state);
    bool shift_pressed = al_key_down(&state, ALLEGRO_KEY_LSHIFT) || al_key_down(&state, ALLEGRO_KEY_RSHIFT);
    auto char_mapping = [shift_pressed] (int keycode) -> char {
        if (keycode == ALLEGRO_KEY_MINUS || keycode == ALLEGRO_KEY_PAD_MINUS)
            return shift_pressed ? '_' : '-';
        if (keycode >= ALLEGRO_KEY_0 && keycode <= ALLEGRO_KEY_9)
            return '0' + keycode - ALLEGRO_KEY_0;
        if (keycode >= ALLEGRO_KEY_PAD_0 && keycode <= ALLEGRO_KEY_PAD_9)
            return '0' + keycode - ALLEGRO_KEY_PAD_0;
        if (keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_Z)
            return (shift_pressed ? 'A' : 'a') + keycode - ALLEGRO_KEY_A;
        return '?';
    };
    return this->Update(char_mapping(keycode));
}

void Engine::TextInput::Update(char charactor) {
    this->Text += charactor;
}

void Engine::TextInput::Update(std::string text) {
    this->Text += text;
}

void Engine::TextInput::Backspace(std::size_t length) {
    // for declarative:
    // this->Text = this->Text.substr(0, std::max(this->Text.length() - length, (std::size_t)0));
    // for imperative:
    for (std::size_t _i = 0; _i < length; _i++) {
        if (this->Text.empty())
            return;
        this->Text.pop_back();
    }
}

void Engine::TextInput::Clear() {
    this->Text.clear();
}

void Engine::TextInput::Replace(std::string text) {
    this->Text = text;
}

void Engine::TextInput::OnKeyDown(int keyCode) {
    this->Update(keyCode);
}
