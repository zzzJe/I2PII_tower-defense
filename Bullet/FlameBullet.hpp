#ifndef FLAMEBULLET_HPP
#define FLAMEBULLET_HPP
#include <allegro5/base.h>

#include "Bullet.hpp"
#include "FlameBullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class FlameBullet : public Bullet {
protected:
	const float rotateRadian = 2 * ALLEGRO_PI;
public:
	explicit FlameBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent);
	void OnExplode(Enemy* enemy) override;
	void Draw() const override;
};
#endif // MISSILEBULLET_HPP
