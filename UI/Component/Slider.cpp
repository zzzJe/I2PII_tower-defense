#include <allegro5/allegro.h>
#include <algorithm>
#include <string>

#include "Engine/Point.hpp"
#include "Slider.hpp"

Slider::Slider(float x, float y, float w, float h) :
    ImageButton("stage-select/slider.png", "stage-select/slider-blue.png", x, y),
    Bar("stage-select/bar.png", x, y, w, h),
    End1("stage-select/end.png", x, y + h / 2, 0, 0, 0.5, 0.5),
    End2("stage-select/end.png", x + w, y + h / 2, 0, 0, 0.5, 0.5) {
    Position.x += w;
    Position.y += h / 2;
    Anchor = Engine::Point(0.5, 0.5);
}
void Slider::Draw() const {
    // DONE: [HACKATHON-3-BUG] (4/5): Draw the nested components here, so they are displayed correctly
    this->Bar.Draw();
    this->End1.Draw();
    this->End2.Draw();
    ImageButton::Draw();
}
void Slider::SetOnValueChangedCallback(std::function<void(float value)> onValueChangedCallback) {
    OnValueChangedCallback = onValueChangedCallback;
}
void Slider::SetValue(float value) {
    if (this->value != value) {
        // DONE: [HACKATHON-3-BUG] (5/5): Fix the function, so it can set the slider value correctly
        Position.x = Bar.Position.x + value * Bar.Size.x;
        this->value = value;
        if (OnValueChangedCallback)
            OnValueChangedCallback(value);
    }
}
void Slider::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && mouseIn)
        Down = true;
}
void Slider::OnMouseUp(int button, int mx, int my) {
    Down = false;
}
void Slider::OnMouseMove(int mx, int my) {
    ImageButton::OnMouseMove(mx, my);
    if (Down) {
        // Clamp
        float clamped = std::min(std::max(static_cast<float>(mx), Bar.Position.x), Bar.Position.x + Bar.Size.x);
        float value = (clamped - Bar.Position.x) / Bar.Size.x * 1.0f;
        SetValue(value);
    }
}
