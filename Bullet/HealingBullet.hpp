#ifndef HEALINGBULLET_HPP
#define HEALINGBULLET_HPP

#include "Bullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class HealingBullet : public Bullet {
public:
	explicit HealingBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
    void OnExplode(Enemy* enemy) override;
};
#endif // LASERBULLET_HPP
