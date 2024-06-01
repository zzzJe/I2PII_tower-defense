// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/LoseScene.hpp"
#include "Scene/PlayScene.hpp"
#include "Scene/StageSelectScene.hpp"
#include "Scene/WinScene.hpp"
#include "Scene/StartScene.hpp"
#include "Scene/SettingsScene.hpp"
#include "Scene/ScoreboardScene.hpp"

int main(int argc, char **argv) {
    Engine::LOG::SetConfig(true);
    Engine::GameEngine& game = Engine::GameEngine::GetInstance();

    // DONE: [HACKATHON-1-SCENE] (3/4): Register Scenes here
    game.AddNewScene("start-scene", new StartScene());
    game.AddNewScene("stage-select", new StageSelectScene());
    game.AddNewScene("settings", new SettingsScene());
    game.AddNewScene("stage-select/scoreboard", new ScoreboardScene());
    game.AddNewScene("play", new PlayScene());
    game.AddNewScene("lose-scene", new LoseScene());
    game.AddNewScene("win-scene", new WinScene());

    // DONE: [HACKATHON-1-SCENE] (4/4): Change the start scene
    game.Start("start-scene", 60, 1600, 832);
    // game.Start("stage-select", 60, 1600, 832);
    // game.Start("settings", 60, 1600, 832);
    return 0;
}
