#include <allegro5/base.h>
#include <random>
#include <string>

#include <allegro5/base.h>
#include <allegro5/allegro.h>
#include "ArmoredVehicleGreen.hpp"

ArmoredVehicleGreen::ArmoredVehicleGreen(int x, int y):
    Enemy("play/enemy-8.png", x, y, 15, 35, 35, ArmoredVehicleGreen::Reward)
{}

void ArmoredVehicleGreen::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    Rotation = ALLEGRO_PI * 1.5 + atan2(Velocity.y, Velocity.x);
}
