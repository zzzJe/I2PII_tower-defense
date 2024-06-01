#ifndef PLANEENEMY_HPP
#define PLANEENEMY_HPP
#include "Enemy.hpp"

class PlaneEnemy : public Enemy {
public:
    static const int Reward = 10;
    PlaneEnemy(int x, int y);
};
#endif // PLANEENEMY_HPP
