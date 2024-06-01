#include <random>
#include <string>

#include "Scene/PlayScene.hpp"
#include "Enemy/CreeperEnemy.hpp"
#include "UI/Animation/ExplosionEffect.hpp"
#include "UI/Animation/DirtyEffect.hpp"

CreeperEnemy::CreeperEnemy(int x, int y):
    Enemy("play/enemy-12.png", x, y, 10, 50, 40, CreeperEnemy::Reward)
{}

void CreeperEnemy::OnExplode() {
    // Remove range 3x3 turrets
    auto outOfBound = [] (Engine::Point p) -> bool {
        return p.x < 0
            || p.x >= PlayScene::MapWidth
            || p.y < 0
            || p.y >= PlayScene::MapHeight;
    };
    const Engine::Point directions[8] = {
        Engine::Point(0, 1),
        Engine::Point(1, 1),
        Engine::Point(1, 0),
        Engine::Point(1, -1),
        Engine::Point(0, -1),
        Engine::Point(-1, -1),
        Engine::Point(-1, 0),
        Engine::Point(-1, 1),
    };
    const Engine::Point selfAt = Engine::Point(
        (int)this->Position.x / PlayScene::BlockSize,
        (int)this->Position.y / PlayScene::BlockSize
    );
    for (auto& direction : directions) {
        const Engine::Point updatePoint = selfAt + direction;
        if (outOfBound(updatePoint))
            continue;
        if (getPlayScene()->TryRemoveOneTurret(updatePoint))
            getPlayScene()->EffectGroup->AddNewObject(new ExplosionEffect(
                updatePoint.x * PlayScene::BlockSize + PlayScene::BlockSize / 2,
                updatePoint.y * PlayScene::BlockSize + PlayScene::BlockSize / 2
            ));
    }
    // Add explosion effect about Creeper
    Enemy::OnExplode();
}

void CreeperEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    Rotation = ALLEGRO_PI * 1.5 + atan2(Velocity.y, Velocity.x);
}
