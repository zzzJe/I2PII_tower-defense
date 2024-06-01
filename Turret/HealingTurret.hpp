#ifndef HEALINGTURRET_HPP
#define HEALINGTURRET_HPP

#include "Turret.hpp"

class HealingTurret: public Turret {
public:
    static const int Price;
    HealingTurret(float x, float y);
    void CreateBullet() override;
};
#endif
