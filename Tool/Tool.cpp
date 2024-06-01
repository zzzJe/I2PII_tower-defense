#include <allegro5/allegro_primitives.h>
#include "Scene/PlayScene.hpp"
#include "Tool/Tool.hpp"

Tool::Tool(std::string imgTool, float x, float y, int price):
    Turret("play/tool-base.png", imgTool, x, y, 0, price, 0)
{}
void Tool::Update(float deltaTime) {}
void Tool::CreateBullet() {}
void Tool::Draw() const {
    if (Preview) {
        al_draw_filled_circle(Position.x, Position.y, CollisionRadius, al_map_rgba(0, 255, 0, 50));
    }
    Sprite::Draw();
    if (PlayScene::DebugMode) {
        // Draw target radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(0, 0, 255), 2);
    }
}
