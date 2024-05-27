#ifndef FLAMEBULLET_HPP
#define FLAMEBULLET_HPP

#include <allegro5/base.h>
#include <list>

#include "Engine/IObject.hpp"
#include "Bullet.hpp"
#include "FlameBullet.hpp"

class Enemy;
class Turret;
namespace Engine {
struct Point;
}  // namespace Engine

class FlameBullet : public Bullet {
private:
	int RemainTicks;
protected:
	const float rotateRadian = 2 * ALLEGRO_PI;
public:
	explicit FlameBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent, double duration);
	void OnExplode(Enemy* enemy) override;
	void Update(float deltaTime) override;
	void Draw() const override;
};
#endif
