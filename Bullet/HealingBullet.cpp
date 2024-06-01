#include <allegro5/base.h>
#include <random>
#include <string>

#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Group.hpp"
#include "HealingBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

class Turret;

HealingBullet::HealingBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent) :
    Bullet("play/bullet-11.png", 800, -0.5, position, forwardDirection, rotation - ALLEGRO_PI / 2, parent) {
}
void HealingBullet::OnExplode(Enemy* enemy) {
    this->getPlayScene()->EarnMoney(1);
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(2, 5);
    getPlayScene()->GroundEffectGroup->AddNewObject(new DirtyEffect("play/dirty-4.png", dist(rng), enemy->Position.x, enemy->Position.y));
}
