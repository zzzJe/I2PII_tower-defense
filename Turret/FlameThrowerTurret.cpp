#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/FlameBullet.hpp"
#include "Engine/Group.hpp"
#include "FlameThrowerTurret.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"

const int FlameThrowerTurret::Price = 50;
FlameThrowerTurret::FlameThrowerTurret(float x, float y):
	Turret("play/tower-base.png", "play/turret-6.png", x, y, 90, Price, 0.01),
	Flame("play/bullet-6.png", x, y, 100, 100)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}
void FlameThrowerTurret::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new FlameBullet(Position, diff, rotation, this));
	AudioHelper::PlayAudio("gun.wav");
}
void FlameThrowerTurret::Update(float deltaTime) {
	Turret::Update(deltaTime);
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	Engine::Point normalized = diff.Normalize();
	this->Flame.Position = this->Position + normalized * 48;
	this->Flame.Rotation = this->Rotation;
}
void FlameThrowerTurret::Draw() const {
	Turret::Draw();
	if (this->Target != nullptr) {
		this->Flame.Draw();
	}
}
