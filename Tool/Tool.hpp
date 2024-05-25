#ifndef TOOL_HPP
#define TOOL_HPP
#include <allegro5/base.h>
#include <list>
#include <string>

#include "Turret/Turret.hpp"
#include "Engine/Sprite.hpp"

class Tool: public Turret {
public:
    Tool(std::string imgTool, float x, float y, int price);
    void Update(float deltaTime) override;
    void CreateBullet() override;
    void Draw() const override;
};
#endif
