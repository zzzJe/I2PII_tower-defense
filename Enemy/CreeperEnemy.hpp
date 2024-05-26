#ifndef CREEPERENEMY_HPP
#define CREEPERENEMY_HPP

#include "Enemy.hpp"

class CreeperEnemy : public Enemy {
public:
	static const int Reward = 20;
	CreeperEnemy(int x, int y);
    void OnExplode() override;
    void Update(float deltaTime) override;
};
#endif
