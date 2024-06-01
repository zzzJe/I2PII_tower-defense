#ifndef FLAMETHROWERTurret_HPP
#define FLAMETHROWERTurret_HPP

#include "Turret.hpp"
#include "Engine/Sprite.hpp"

class FlameThrowerTurret: public Turret {
private:
    Engine::Sprite Flame;
public:
    static const int Price;
    FlameThrowerTurret(float x, float y);
    void CreateBullet() override;
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif
