#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <functional>
#include <fstream>
#include <vector>
#include <utility>
#include <string>

#include "Engine/Resources.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/ImageButton.hpp"
#include "ScoreboardScene.hpp"

void ScoreboardScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;

    Engine::ImageButton* btn;
#define BTN_WIDTH 400
#define BTN_HEIGHT 100
    btn = new Engine::ImageButton("settings/dirt.png", "settings/floor.png", halfW, h * 0.9, BTN_WIDTH, BTN_HEIGHT, 0.5, 0.5);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::BackOnClick, this, 0));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, h * 0.9, 0, 0, 0, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("settings/dirt.png", "settings/floor.png", halfW * 0.4, h * 0.9, BTN_WIDTH, BTN_HEIGHT, 0.5, 0.5);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::ChangeToPrevOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Prev Page", "pirulen.ttf", 48, halfW * 0.4, h * 0.9, 0, 0, 0, 255, 0.5, 0.5));
    
    btn = new Engine::ImageButton("settings/dirt.png", "settings/floor.png", halfW * 1.6, h * 0.9, BTN_WIDTH, BTN_HEIGHT, 0.5, 0.5);
    btn->SetOnClickCallback(std::bind(&ScoreboardScene::ChangeToNextBackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("Next Page", "pirulen.ttf", 48, halfW * 1.6, h * 0.9, 0, 0, 0, 255, 0.5, 0.5));
#undef BTN_WIDTH
#undef BTN_HEIGHT

    AddNewObject(new Engine::Label("HIGHSCORE BOARD", "pirulen.ttf", 48, halfW, 58, 255, 255, 255, 255, 0.5, 0.5));

    const int mapMaps[2] = {1, 2};
    for (auto mapId : mapMaps) {
        std::string dataFilePath = "Resource/highscores/" + std::to_string(mapId) + ".score";
        std::ifstream fin(dataFilePath);
        if (!fin.is_open())
            throw "Open \"Resource/highscores/" + std::to_string(mapId) + ".score\" failed";
        while (true) {
            std::string name;
            std::string score;
            std::string date;
            std::getline(fin, name, '\t');
            std::getline(fin, score, '\t');
            std::getline(fin, date, '\n');
            if (fin.eof())
                break;
            this->HighscoreData.push_back(std::tuple<int, std::string, std::string, std::string>(mapId, name, score, date));
        }
        fin.close();
    }
    this->TotalPageNumbers = (this->HighscoreData.size() - 1) / this->RowNumbers + 1;
}

void ScoreboardScene::BackOnClick(int stage) {
    Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}

void ScoreboardScene::ChangeToPrevOnClick() {
    if (this->CurrentIndex != 1)
        this->CurrentIndex--;
}

void ScoreboardScene::ChangeToNextBackOnClick() {
    if (this->CurrentIndex != this->TotalPageNumbers)
        this->CurrentIndex++;
}
void ScoreboardScene::Draw() const {
    IScene::Draw();
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int dataBeginShift = (this->CurrentIndex - 1) * this->RowNumbers;
    auto sliceBegin = this->HighscoreData.cbegin() + dataBeginShift;
    auto sliceEnd = (sliceBegin + this->RowNumbers < this->HighscoreData.cend())
        ? sliceBegin + this->RowNumbers
        : this->HighscoreData.cend();
    auto toDisplayData = std::vector<std::tuple<int, std::string, std::string, std::string>>(
        sliceBegin,
        sliceEnd
    );
#define OFFSET_X 220
#define OFFSET_Y 150
    std::shared_ptr<ALLEGRO_FONT> font = Engine::Resources::GetInstance().GetFont("pirulen.ttf", 30);
    ALLEGRO_COLOR headerColor = {
        .r = .8,
        .g = .8,
        .b = .8,
        .a = 1,
    };
    ALLEGRO_COLOR textColor = {
        .r = 1,
        .g = 1,
        .b = 1,
        .a = 1,
    };
    al_draw_text(font.get(), headerColor, OFFSET_X + 0  , OFFSET_Y - 30, ALLEGRO_ALIGN_LEFT, "Map Id");
    al_draw_text(font.get(), headerColor, OFFSET_X + 200, OFFSET_Y - 30, ALLEGRO_ALIGN_LEFT, "Player Name");
    al_draw_text(font.get(), headerColor, OFFSET_X + 550, OFFSET_Y - 30, ALLEGRO_ALIGN_LEFT, "Score");
    al_draw_text(font.get(), headerColor, OFFSET_X + 750, OFFSET_Y - 30, ALLEGRO_ALIGN_LEFT, "Date");
    al_draw_line(w * 0.1, OFFSET_Y + 30, w * 0.9, OFFSET_Y + 30, headerColor, 1);
    for (int i = 0; i < toDisplayData.size(); i++) {
        std::string label = std::to_string(std::get<0>(toDisplayData[i]));
        std::string name = std::get<1>(toDisplayData[i]);
        std::string score = std::get<2>(toDisplayData[i]);
        std::string date = std::get<3>(toDisplayData[i]);
        al_draw_text(font.get(), textColor, OFFSET_X + 0  , OFFSET_Y + (i + 1) * 60, ALLEGRO_ALIGN_LEFT, label.c_str());
        al_draw_text(font.get(), textColor, OFFSET_X + 200, OFFSET_Y + (i + 1) * 60, ALLEGRO_ALIGN_LEFT, name.c_str());
        al_draw_text(font.get(), textColor, OFFSET_X + 550, OFFSET_Y + (i + 1) * 60, ALLEGRO_ALIGN_LEFT, score.c_str());
        al_draw_text(font.get(), textColor, OFFSET_X + 750, OFFSET_Y + (i + 1) * 60, ALLEGRO_ALIGN_LEFT, date.c_str());
    }
#undef OFFSET_X
#undef OFFSET_Y
}

void ScoreboardScene::Terminate() {
    this->HighscoreData.clear();
    this->CurrentIndex = 1;
}
