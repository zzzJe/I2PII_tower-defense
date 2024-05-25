#include "Tool/Shovel.hpp"

const int ShovelTool::Price = 0;

ShovelTool::ShovelTool(float x, float y):
    Tool("play/shovel.png", x, y, 0)
{
    Tool::Turret::Sprite::Rotation = ALLEGRO_PI / 4;
}
