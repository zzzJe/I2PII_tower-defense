#ifndef ARMOREDVEHICLEGREEN_HPP
#define ARMOREDVEHICLEGREEN_HPP
#include "Enemy.hpp"

class ArmoredVehicleGreen : public Enemy {
public:
    static const int Reward = 30;
    ArmoredVehicleGreen(int x, int y);
    void Update(float deltaTime) override;
};
#endif
