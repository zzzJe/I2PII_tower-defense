#ifndef ScoreboardScene_HPP
#define ScoreboardScene_HPP

#include <vector>
#include <utility>
#include <string>

#include "Engine/IScene.hpp"

class ScoreboardScene : public Engine::IScene {
private:
    int CurrentIndex = 1;
    int TotalPageNumbers = 1;
    const int RowNumbers = 8;
    std::vector<std::tuple<int, std::string, std::string, std::string>> HighscoreData;
public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void BackOnClick(int stage);
    void ChangeToPrevOnClick();
    void ChangeToNextBackOnClick();
    void Draw() const override;
    void Terminate() override;
};

#endif
