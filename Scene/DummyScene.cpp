#include "Engine/GameEngine.hpp"
#include "DummyScene.hpp"
#include "Engine/Point.hpp"

#include "UI/Component/Label.hpp"
#include "UI/Component/TextInput.hpp"
#include "UI/Component/ChatRoom.hpp"

void DummyScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    // AddNewObject(new Engine::Label("Dummy Scene", "pirulen.ttf", 48, halfW, 50, 255, 255, 255, 255, 0.5, 0.5));
    AddNewControlObject(this->room = new Engine::ChatRoom(
        150, 50, w - 300, h - 100, 0, 0, 50, 150, 10, 10, 5, "Consolas.ttf", 24, "Consolas.ttf", 30
    ));
    AddNewControlObject(this->input = new Engine::TextInput("", "Consolas.ttf", -1, halfW, h - 100, 900, 60, 0.1, true, false, 3, 5, 0.5, 0.5, {32, 32, 32, 255}, {1, 1, 1, 1}, {0.5, 0.5, 0.5, 1}));
}

void DummyScene::Terminate() {
    IScene::Terminate();
}

void DummyScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_UP)
        room->UpdateView(5);
    if (keyCode == ALLEGRO_KEY_DOWN)
        room->UpdateView(-5);
    if (keyCode == ALLEGRO_KEY_TAB)
        this->input->Focus();
    if (keyCode == ALLEGRO_KEY_ENTER) {
        for (char c : this->input->Text) {
            if (c != ' ') {
                this->room->Append("CFdark", this->input->Text);
                break;
            }
        }
        this->input->Clear();
    }
    if (keyCode == ALLEGRO_KEY_7)
        this->room->ToggleVisible(false);
    if (keyCode == ALLEGRO_KEY_8)
        this->room->ToggleVisible(true);
    if (keyCode == ALLEGRO_KEY_5)
        this->input->ToggleEnable(false);
    if (keyCode == ALLEGRO_KEY_6)
        this->input->ToggleEnable(true);
    if (keyCode == ALLEGRO_KEY_9)
        this->input->ToggleVisible(false);
    if (keyCode == ALLEGRO_KEY_0)
        this->input->ToggleVisible(true);
}
