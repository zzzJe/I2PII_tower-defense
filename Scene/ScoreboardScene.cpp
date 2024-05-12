#include <functional>
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "ScoreboardScene.hpp"

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton *btn;
#define BACK_BTN_WIDTH 400
#define BACK_BTN_HEIGHT 100
    btn = new Engine::ImageButton("settings/dirt.png", "settings/floor.png", std::min((int)(w * 0.7), w - BACK_BTN_WIDTH), std::min((int)(h * 0.75), h - BACK_BTN_HEIGHT), BACK_BTN_WIDTH, BACK_BTN_HEIGHT);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 0));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, std::min((int)(w * 0.7), w - BACK_BTN_WIDTH) + BACK_BTN_WIDTH / 2, std::min((int)(h * 0.75), h - BACK_BTN_HEIGHT) + BACK_BTN_HEIGHT / 2, 0, 0, 0, 255, 0.5, 0.5));
#undef BACK_BTN_WIDTH
#undef BACK_BTN_HEIGHT

    AddNewObject(new Engine::Label("HIGHSCORE BOARD", "pirulen.ttf", 48, halfW, 58, 255, 255, 255, 255, 0.5, 0.5));
}

void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void ScoreboardScene::ChangeToPrevOnClick() {}

void ScoreboardScene::ChangeToNextBackOnClick() {}

