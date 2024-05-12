#ifndef ScoreboardScene_HPP
#define ScoreboardScene_HPP

#include "Engine/IScene.hpp"

class ScoreboardScene : public Engine::IScene {
public:
    explicit ScoreboardScene() = default;
    void Initialize() override;
    void BackOnClick(int stage);
    void ChangeToPrevOnClick();
    void ChangeToNextBackOnClick();
};

#endif
