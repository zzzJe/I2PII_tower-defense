#ifndef WINSCENE_HPP
#define WINSCENE_HPP
#include <allegro5/allegro_audio.h>
#include "Engine/IScene.hpp"
#include "UI/Component/TextInput.hpp"

class WinScene final : public Engine::IScene {
private:
	float ticks;
	ALLEGRO_SAMPLE_ID bgmId;
public:
	explicit WinScene() = default;
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void BackOnClick(int stage);
	void temp();
	void SaveOnClick(const int mapId, Engine::TextInput* inputSlot, const int lives, const int money, const int maxMoney);
	PlayScene* GetPlayScene() const;
};

#endif // WINSCENE_HPP
