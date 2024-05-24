#include <fstream>
#include <list>
#include <algorithm>
#include <utility>
#include <functional>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/Image.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "UI/Component/TextInput.hpp"
#include "PlayScene.hpp"
#include "Engine/Point.hpp"
#include "WinScene.hpp"

void WinScene::Initialize() {
	ticks = 0;
	int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
	int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
	int halfW = w / 2;
	int halfH = h / 2;
	AddNewObject(new Engine::Image("win/benjamin-sad.png", halfW * 0.6, halfH, 0, 0, 0.5, 0.5));
	AddNewObject(new Engine::Label("You Win!", "pirulen.ttf", 48, halfW, halfH / 4 - 10, 255, 255, 255, 255, 0.5, 0.5));
	Engine::ImageButton* btn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW - 200, halfH * 7 / 4 - 50, 400, 100);
	btn->SetOnClickCallback(std::bind(&WinScene::BackOnClick, this, 2));
	AddNewControlObject(btn);
	AddNewObject(new Engine::Label("Back", "pirulen.ttf", 48, halfW, halfH * 7 / 4, 0, 0, 0, 255, 0.5, 0.5));
	Engine::TextInput* input = new Engine::TextInput("nam", "pirulen.ttf", 10, halfW * 1.4, halfH, 500, 60, true, 3, 10, 0.5, 0.5, {32, 32, 32, 255}, {1, 1, 1, 1}, {0.5, 0.5, 0.5, 1});
	AddNewObject(new Engine::Label("Input name:", "pirulen.ttf", 40, halfW * 1.4 - input->GetWidth() / 2, halfH - input->GetFullHeight(), 255, 255, 255, 255, 0, 0));
	AddNewControlObject(input);
	Engine::ImageButton* saveBtn = new Engine::ImageButton("win/dirt.png", "win/floor.png", halfW * 1.4 - input->GetWidth() / 2, halfH + input->GetFullHeight() * .75, 200, 50);
	saveBtn->SetOnClickCallback([=] () {
		int mapId = this->GetPlayScene()->MapId;
		int lives = this->GetPlayScene()->GetLives();
		int money = this->GetPlayScene()->GetMoney();
		int maxMoney = this->GetPlayScene()->GetMaxMoney();
		this->SaveOnClick(mapId, input, lives, money, maxMoney);
	});
	AddNewControlObject(saveBtn);
	AddNewObject(new Engine::Label("OK", "pirulen.ttf", 36, halfW * 1.4 - input->GetWidth() / 2 + 100, halfH + input->GetFullHeight() * 0.75 + 25, 0, 0, 0, 255, 0.5, 0.5));
	bgmId = AudioHelper::PlayAudio("win.wav");
}
void WinScene::Terminate() {
	IScene::Terminate();
	AudioHelper::StopBGM(bgmId);
}
void WinScene::Update(float deltaTime) {
	ticks += deltaTime;
	if (
		ticks > 4 && ticks < 100
		&& dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"))->MapId == 2)
	{
		ticks = 100;
		bgmId = AudioHelper::PlayBGM("happy.ogg");
	}
}
void WinScene::BackOnClick(int stage) {
	// Change to select scene.
	Engine::GameEngine::GetInstance().ChangeScene("stage-select");
}
void WinScene::SaveOnClick(const int mapId, Engine::TextInput* inputSlot, const int lives, const int money, const int maxMoney) {
	if (inputSlot->IsConsumedSlot())
		return;
	if (inputSlot->IsInvalidContent())
		return;
	std::string dataFilePath = "Resource/highscores/" + std::to_string(mapId) + ".score";
	std::ifstream fin(dataFilePath);
	if (!fin.is_open())
		throw "Open \"Resource/highscores/" + std::to_string(mapId) + ".score\" failed";
	std::list<std::tuple<std::string, std::string, std::string>> playerDataList;
	while (true) {
		std::string name;
		std::string score;
		std::string date;
		std::getline(fin, name, '\t');
		std::getline(fin, score, '\t');
		std::getline(fin, date, '\n');
		if (fin.eof())
			break;
		playerDataList.push_back(std::tuple<std::string, std::string, std::string>(name, score, date));
	}
	int userScore = lives * 500 + (int)money;
	auto getFormattedTime = [] () -> std::string {
		auto nowTime = std::chrono::system_clock::now();
		std::time_t cStyleNowTime = std::chrono::system_clock::to_time_t(nowTime);
		std::tm nowTm = *std::gmtime(&cStyleNowTime);
		std::ostringstream oss;
		oss << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S");
		return oss.str();
	};
	auto insert_at = std::find_if(playerDataList.cbegin(), playerDataList.cend(), [=] (auto t) {
		return stoi(std::get<1>(t)) < userScore;
	});
	playerDataList.insert(insert_at, std::tuple<std::string, std::string, std::string>(inputSlot->Text, std::to_string(userScore), getFormattedTime()));
	fin.close();
	std::ofstream fout(dataFilePath, std::ofstream::out);
	for (auto playerData : playerDataList)
		fout << std::get<0>(playerData) << "\t" << std::get<1>(playerData) << "\t" << std::get<2>(playerData) << "\n";
	fout.close();
	inputSlot->ConsumeSlot();
}
PlayScene* WinScene::GetPlayScene() const {
	return dynamic_cast<PlayScene*>(Engine::GameEngine::GetInstance().GetScene("play"));
}
void WinScene::temp() {
	// std::cout << this->text->Text << std::endl;
}