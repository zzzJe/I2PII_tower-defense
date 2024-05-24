#include <allegro5/base.h>
#include <cmath>
#include <string>

#include "Engine/AudioHelper.hpp"
#include "Bullet/HealingBullet.hpp"
#include "Engine/Group.hpp"
#include "HealingTurrent.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"

const int HealingTurrent::Price = 50;
HealingTurrent::HealingTurrent(float x, float y) :
	Turret("play/tower-base.png", "play/turret-8.png", x, y, 150, Price, 0.8)
{
	// Move center downward, since we the turret head is slightly biased upward.
	Anchor.y += 8.0f / GetBitmapHeight();
}
void HealingTurrent::CreateBullet() {
	Engine::Point diff = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
	float rotation = atan2(diff.y, diff.x);
	Engine::Point normalized = diff.Normalize();
	// Change bullet position to the front of the gun barrel.
	getPlayScene()->BulletGroup->AddNewObject(new HealingBullet(Position + normalized * 36, diff, rotation, this));
	AudioHelper::PlayAudio("gun.wav");
}
