#ifndef DUMMYSCENE_HPP
#define DUMMYSCENE_HPP
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "UI/Component/ChatRoom.hpp"
#include "UI/Component/TextInput.hpp"

class DummyScene final : public Engine::IScene {
private:
    Engine::ChatRoom* room;
    Engine::TextInput* input;
public:
    explicit DummyScene() = default;
    void Initialize() override;
    void Terminate() override;
    void OnKeyDown(int keyCode) override;
};

#endif // DUMMYSCENE_HPP
