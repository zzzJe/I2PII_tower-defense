#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_primitives.h>
#include "Engine/Collider.hpp"
#include "Engine/Point.hpp"
#include "UI/Component/TextInput.hpp"

Engine::TextInput::TextInput(
    const std::string& data, const std::string& font, int maxLength,
    float x, float y, float width, float height, float fontShiftRatio,
    bool allowWhiteSpace, bool roundCorner, float margin, float borderWidth, float anchorX, float anchorY,
    ALLEGRO_COLOR textColor,
    ALLEGRO_COLOR backgroundColor,
    ALLEGRO_COLOR borderColor,
    ALLEGRO_COLOR invalidColor,
    ALLEGRO_COLOR editingColor
):
    Label(data, font, height * 0.9, x, y, textColor.r, textColor.g, textColor.b, textColor.a, anchorX, anchorY),
    MaxLength(maxLength),
    Width(width),
    Height(height),
    Margin(margin),
    BorderWidth(borderWidth),
    FontShiftRatio(fontShiftRatio),
    RoundCorner(roundCorner),
    BackgroundColor(backgroundColor),
    BorderColor(borderColor),
    InvalidColor(invalidColor),
    EditingColor(editingColor),
    InvalidDisplay(false),
    Used(false),
    Editing(false),
    AllowWhiteSpace(allowWhiteSpace)
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
    const ALLEGRO_COLOR& borderDisplayColor = this->IsInvalidDisplay()
        ? this->InvalidColor
        : this->Editing
        ? this->EditingColor
        : this->BorderColor;
    if (this->RoundCorner) {
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            borderDisplayColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            borderDisplayColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            borderDisplayColor,
            this->BorderWidth
        );
        al_draw_line(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            borderDisplayColor,
            this->BorderWidth
        );
        al_draw_filled_circle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            borderDisplayColor
        );
        al_draw_filled_circle(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            borderDisplayColor
        );
        al_draw_filled_circle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            borderDisplayColor
        );
        al_draw_filled_circle(
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->BorderWidth / 2.,
            borderDisplayColor
        );
    } else {
        al_draw_rectangle(
            this->Position.x + this->BorderWidth * .5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth * .5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            this->Position.x + this->Width + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->Height + this->Margin * 2 + this->BorderWidth * 1.5 - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth),
            borderDisplayColor,
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
                this->Position.y + this->BorderWidth + this->Margin - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth) + this->Height * this->FontShiftRatio,
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
            this->Position.y + this->BorderWidth + this->Margin - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth) + this->Height * this->FontShiftRatio,
            ALLEGRO_ALIGN_LEFT,
            this->Text.c_str()
        );
    }
}

void Engine::TextInput::Update(int keycode) {
    if (keycode == ALLEGRO_KEY_BACKSPACE)
        return this->Backspace();
    auto is_valid_inputcode = [=] (int keycode) {
        return keycode >= ALLEGRO_KEY_A && keycode <= ALLEGRO_KEY_PAD_9
            || keycode == ALLEGRO_KEY_MINUS
            || keycode == ALLEGRO_KEY_PAD_MINUS
            || keycode == ALLEGRO_KEY_FULLSTOP
            || keycode == ALLEGRO_KEY_PAD_DELETE
            || AllowWhiteSpace && keycode == ALLEGRO_KEY_SPACE;
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
        if (keycode == ALLEGRO_KEY_FULLSTOP || keycode == ALLEGRO_KEY_PAD_DELETE)
            return '.';
        if (keycode == ALLEGRO_KEY_SPACE)
            return ' ';
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

void Engine::TextInput::OnMouseDown(int button, int mx, int my) {
    this->Editing = Engine::Collider::IsPointInRect(
        Engine::Point(mx, my),
        Engine::Point(
            this->Position.x + this->BorderWidth - this->Anchor.x * (this->Width + this->Margin + this->BorderWidth),
            this->Position.y + this->BorderWidth - this->Anchor.y * (this->Height + this->Margin + this->BorderWidth)
        ),
        Engine::Point(
            this->Width + this->Margin * 2,
            this->Height + this->Margin * 2
        )
    );
    if (this->Editing)
        this->InvalidDisplay = false;
}

void Engine::TextInput::OnKeyDown(int keyCode) {
    if (this->Used || !this->Editing)
        return;
    if (keyCode == ALLEGRO_KEY_ENTER || keyCode == ALLEGRO_KEY_PAD_ENTER) {
        this->Editing = false;
        return;
    }
    if (keyCode == ALLEGRO_KEY_BACKSPACE) {
        this->Backspace();
        this->InvalidDisplay = false;
    } else if (this->MaxLength != this->Text.length()) {
        this->Update(keyCode);
        this->InvalidDisplay = false;
    } else {
        this->InvalidDisplay = true;
    }
}

void Engine::TextInput::Focus(bool state) {
    this->Editing = state;
}

bool Engine::TextInput::IsInvalidContent() const {
    return this->Text.empty() || this->MaxLength < this->Text.length();
}

bool Engine::TextInput::IsInvalidDisplay() const {
    return this->InvalidDisplay;
}

bool Engine::TextInput::ConsumeSlot() {
    if (this->Used)
        return true;
    this->Used = true;
    return false;
}

bool Engine::TextInput::IsConsumedSlot() const {
    return this->Used;
}
