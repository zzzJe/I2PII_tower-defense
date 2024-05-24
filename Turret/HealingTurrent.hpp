#ifndef HEALINGTURRET_HPP
#define HEALINGTURRET_HPP

#include "Turret.hpp"

class HealingTurrent: public Turret {
public:
	static const int Price;
    HealingTurrent(float x, float y);
	void CreateBullet() override;
};
#endif
