#include <allegro5/allegro.h>
#include <random>
#include <string>

#include "FlameBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "UI/Animation/DirtyEffect.hpp"

FlameBullet::FlameBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent):
    Bullet("play/bullet-6.png", 0, 0.1, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent)
{
    this->Bullet::Sprite::CollisionRadius = 100;
}

void FlameBullet::OnExplode(Enemy* enemy) {
    std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
	getPlayScene()->EffectGroup->AddNewObject(new DirtyEffect("play/flame.png", dist(rng), enemy->Position.x, enemy->Position.y));
}

void FlameBullet::Draw() const {
    // al_draw_tinted_scaled_rotated_bitmap(bmp.get(), Tint, Anchor.x * GetBitmapWidth(), Anchor.y * GetBitmapHeight(),
	// 		Position.x, Position.y, Size.x / GetBitmapWidth() * 2, Size.y / GetBitmapHeight() * 2, Rotation, 0);
}
