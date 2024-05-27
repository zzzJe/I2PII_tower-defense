#include <allegro5/allegro.h>
#include <algorithm>
#include <random>
#include <string>

#include "FlameBullet.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Animation/DirtyEffect.hpp"

FlameBullet::FlameBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent, double duration):
    Bullet("play/bullet-6.png", 0, 0.15, position, forwardDirection, rotation + ALLEGRO_PI / 2, parent),
    RemainTicks(std::max(static_cast<int>(duration * Engine::GameEngine::GetInstance().GetFps()), 0))
{
    this->Bullet::Sprite::CollisionRadius = 45;
}

void FlameBullet::OnExplode(Enemy* enemy) {
    std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 1);
	getPlayScene()->EffectGroup->AddNewObject(new DirtyEffect("play/flame.png", dist(rng), enemy->Position.x, enemy->Position.y));
}

void FlameBullet::Update(float deltaTime) {
    if (this->RemainTicks == -1) {
        getPlayScene()->RemoveObject(this->objectIterator);
    } else {
        this->RemainTicks--;
        Bullet::Update(deltaTime);
    }
}

void FlameBullet::Draw() const {
    al_draw_tinted_scaled_rotated_bitmap(bmp.get(), Tint, Anchor.x * GetBitmapWidth(), Anchor.y * GetBitmapHeight(),
			Position.x, Position.y, Size.x / GetBitmapWidth() * 3, Size.y / GetBitmapHeight() * 3, Rotation, 0);
}
