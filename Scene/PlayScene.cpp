#include <allegro5/allegro.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <vector>
#include <queue>
#include <string>
#include <memory>

#include "Engine/AudioHelper.hpp"
#include "UI/Animation/DirtyEffect.hpp"
#include "Enemy/Enemy.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "UI/Component/Label.hpp"
#include "Turret/LaserTurret.hpp"
#include "Turret/MachineGunTurret.hpp"
#include "Turret/MissileTurret.hpp"
#include "Turret/HealingTurret.hpp"
#include "Turret/FlameThrowerTurret.hpp"
#include "Tool/Shovel.hpp"
#include "UI/Animation/Plane.hpp"
#include "Enemy/PlaneEnemy.hpp"
#include "PlayScene.hpp"
#include "Engine/Resources.hpp"
#include "Enemy/SoldierEnemy.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Enemy/ArmoredVehicleGreen.hpp"
#include "Enemy/CreeperEnemy.hpp"
#include "Turret/TurretButton.hpp"

bool PlayScene::DebugMode = false;
const std::vector<Engine::Point> PlayScene::directions = { Engine::Point(-1, 0), Engine::Point(0, -1), Engine::Point(1, 0), Engine::Point(0, 1) };
const int PlayScene::MapWidth = 20, PlayScene::MapHeight = 13;
const int PlayScene::BlockSize = 64;
const float PlayScene::DangerTime = 7.61;
const Engine::Point PlayScene::SpawnGridPoint = Engine::Point(-1, 0);
const Engine::Point PlayScene::EndGridPoint = Engine::Point(MapWidth, MapHeight - 1);
const std::vector<int> PlayScene::code = { ALLEGRO_KEY_UP, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN, ALLEGRO_KEY_DOWN,
                                    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_RIGHT,
                                    ALLEGRO_KEY_B, ALLEGRO_KEY_A, ALLEGRO_KEYMOD_SHIFT, ALLEGRO_KEY_ENTER };
Engine::Point PlayScene::GetClientSize() {
    return Engine::Point(MapWidth * BlockSize, MapHeight * BlockSize);
}
void PlayScene::Initialize() {
    // DONE: [HACKATHON-3-BUG] (1/5): There's a bug in this file, which crashes the game when you lose. Try to find it.
    // DONE: [HACKATHON-3-BUG] (2/5): Find out the cheat code to test.
    // DONE: [HACKATHON-3-BUG] (2/5): It should generate a Plane, and add 10000 to the money, but it doesn't work now.
    keyStrokes.clear();
    ticks = 0;
    deathCountDown = -1;
    lives = 10;
    money = PlayScene::InitialMoney;
    SpeedMult = 1;
    // Add groups from bottom to top.
    AddNewObject(TileMapGroup = new Group());
    AddNewObject(GroundEffectGroup = new Group());
    AddNewObject(DebugIndicatorGroup = new Group());
    AddNewObject(TowerGroup = new Group());
    AddNewObject(EnemyGroup = new Group());
    AddNewObject(BulletGroup = new Group());
    AddNewObject(EffectGroup = new Group());
    // Should support buttons.
    AddNewControlObject(UIGroup = new Group());
    InitializeMapAssociatedStates();
    ReadEnemyWave();
    ConstructUI();
    imgTarget = new Engine::Image("play/target.png", 0, 0);
    imgTarget->Visible = false;
    preview = nullptr;
    UIGroup->AddNewObject(imgTarget);
    // Preload Lose Scene
    deathBGMInstance = Engine::Resources::GetInstance().GetSampleInstance("astronomia.ogg");
    Engine::Resources::GetInstance().GetBitmap("lose/benjamin-happy.png");
    // Start BGM.
    bgmId = AudioHelper::PlayBGM("play.ogg");
}
void PlayScene::Terminate() {
    AudioHelper::StopBGM(bgmId);
    AudioHelper::StopSample(deathBGMInstance);
    deathBGMInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void PlayScene::Update(float deltaTime) {
    // If we use deltaTime directly, then we might have Bullet-through-paper problem.
    // Reference: Bullet-Through-Paper
    if (SpeedMult == 0)
        deathCountDown = -1;
    else if (deathCountDown != -1)
        SpeedMult = 1;
    // Calculate danger zone.
    std::vector<float> reachEndTimes;
    for (auto& it : EnemyGroup->GetObjects()) {
        reachEndTimes.push_back(dynamic_cast<Enemy*>(it)->reachEndTime);
    }
    // Can use Heap / Priority-Queue instead. But since we won't have too many enemies, sorting is fast enough.
    std::sort(reachEndTimes.begin(), reachEndTimes.end());
    float newDeathCountDown = -1;
    int danger = lives;
    for (auto& it : reachEndTimes) {
        if (it <= DangerTime) {
            danger--;
            if (danger <= 0) {
                // Death Countdown
                float pos = DangerTime - it;
                if (it > deathCountDown) {
                    // Restart Death Count Down BGM.
                    AudioHelper::StopSample(deathBGMInstance);
                    if (SpeedMult != 0)
                        deathBGMInstance = AudioHelper::PlaySample("astronomia.ogg", false, AudioHelper::BGMVolume, pos);
                }
                float alpha = pos / DangerTime;
                alpha = std::max(0, std::min(255, static_cast<int>(alpha * alpha * 255)));
                dangerIndicator->Tint = al_map_rgba(255, 255, 255, alpha);
                newDeathCountDown = it;
                break;
            }
        }
    }
    deathCountDown = newDeathCountDown;
    if (SpeedMult == 0)
        AudioHelper::StopSample(deathBGMInstance);
    if (deathCountDown == -1 && lives > 0) {
        AudioHelper::StopSample(deathBGMInstance);
        dangerIndicator->Tint.a = 0;
    }
    if (SpeedMult == 0)
        deathCountDown = -1;
    for (int i = 0; i < SpeedMult; i++) {
        IScene::Update(deltaTime);
        // Check if we should create new enemy.
        ticks += deltaTime;
        if (enemyWaveData.empty()) {
            if (EnemyGroup->GetObjects().empty()) {
                Engine::GameEngine::GetInstance().ChangeScene("win-scene");
            }
            continue;
        }
        auto current = enemyWaveData.front();
        if (ticks < current.second)
            continue;
        ticks -= current.second;
        enemyWaveData.pop_front();
        const Engine::Point SpawnCoordinate = Engine::Point(SpawnGridPoint.x * BlockSize + BlockSize / 2, SpawnGridPoint.y * BlockSize + BlockSize / 2);
        Enemy* enemy;
        switch (current.first) {
        case 1:
            EnemyGroup->AddNewObject(enemy = new SoldierEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
            break;
        case 2:
            EnemyGroup->AddNewObject(enemy = new PlaneEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
            break;
        case 3:
            EnemyGroup->AddNewObject(enemy = new TankEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
            break;
        case 4:
            EnemyGroup->AddNewObject(enemy = new ArmoredVehicleGreen(SpawnCoordinate.x, SpawnCoordinate.y));
            break;
        case 5:
            EnemyGroup->AddNewObject(enemy = new CreeperEnemy(SpawnCoordinate.x, SpawnCoordinate.y));
            break;
        // TODO: [CUSTOM-ENEMY]: You need to modify 'Resource/enemy1.txt', or 'Resource/enemy2.txt' to spawn the 4th enemy.
        //         The format is "[EnemyId] [TimeDelay] [Repeat]".
        // TODO: [CUSTOM-ENEMY]: Enable the creation of the enemy.
        default:
            continue;
        }
        enemy->UpdatePath(mapDistance);
        // Compensate the time lost.
        enemy->Update(ticks);
    }
    if (preview) {
        preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
        // To keep responding when paused.
        preview->Update(deltaTime);
    }
}
void PlayScene::Draw() const {
    IScene::Draw();
    if (DebugMode) {
        for (int i = 0; i < MapHeight; i++) {
            for (int j = 0; j < MapWidth; j++) {
                {
                    // Draw reverse BFS distance on all reachable blocks.
                    if (mapDistance[i][j] != -1) {
                        // Not elegant nor efficient, but it's quite enough for debugging.
                        Engine::Label label(std::to_string(mapDistance[i][j]), "pirulen.ttf", 28, (j + 0.5) * BlockSize, (i + 0.4) * BlockSize);
                        label.Anchor = Engine::Point(0.5, 0.5);
                        label.Draw();
                    }
                } {
                    // Draw economic value on all blocks.
                    Engine::Label label(std::to_string(economyMap[i][j]), "pirulen.ttf", 16, (j + 1) * BlockSize, (i + 0.85) * BlockSize, 0, 128, 0);
                    label.Anchor = Engine::Point(1, 0.5);
                    label.Draw();
                } {
                    // Draw turret price on all blocks.
                    Engine::Label label(std::to_string(turretPriceState[i][j]), "pirulen.ttf", 16, (j + 1) * BlockSize, (i + 0.65) * BlockSize, 128, 0, 0);
                    label.Anchor = Engine::Point(1, 0.5);
                    label.Draw();
                }
            }
        }
    }
}
bool PlayScene::TryAddOneTurret(Engine::Point p, Turret* t) {
    if (mapState[p.y][p.x] == TILE_OCCUPIED)
        return false;
    // Purchase
    EarnMoney(-t->GetPrice());
    // Add turret to group
    TowerGroup->AddNewObject(t);
    // Update turretPriceState
    turretPriceState[p.y][p.x] = t->GetPrice();
    // Update economyMap
    economyMap[p.y][p.x] = -1;
    auto outOfBound = [] (Engine::Point p) -> bool {
        return p.x < 0
            || p.x >= MapWidth
            || p.y < 0
            || p.y >= MapHeight;
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
    for (auto direction : directions) {
        const Engine::Point updatePoint = p + direction;
        if (outOfBound(updatePoint))
            continue;
        if (economyMap[updatePoint.y][updatePoint.x] == -1)
            continue;
        economyMap[updatePoint.y][updatePoint.x] += t->GetPrice();
    }
    mapState[p.y][p.x] = TILE_OCCUPIED;
    // TODO:
    // After the current turret is placed, we should use BFS to iterate 'should hollow' block
    // 'should hollow' block is a block that is to be inside a wall of turrets
    // after appropriate mapDistance update, the 'should hollow' block should have
    //  1. mapDistance == -1
    //  2. economyMap != -1
    //  ?. BFS should be enough for possible case...?
    return true;
}
bool PlayScene::TryRemoveOneTurret(Engine::Point p) {
    if (mapState[p.y][p.x] != TILE_OCCUPIED)
        return false;
    // Remove turret from group
    for (auto& tower : TowerGroup->GetObjects()) {
        if (tower->Position == p * BlockSize + Engine::Point(BlockSize, BlockSize) / 2) {
            TowerGroup->RemoveObject(tower->GetObjectIterator());
            break;
        }
    }
    mapState[p.y][p.x] = originalMapState[p.y][p.x];
    mapDistance = CalculateBFSDistance();
    // Update economyMap
    auto outOfBound = [] (Engine::Point p) -> bool {
        return p.x < 0
            || p.x >= MapWidth
            || p.y < 0
            || p.y >= MapHeight;
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
    if (mapDistance[p.y][p.x] != -1)
        economyMap[p.y][p.x] = 0;
    for (auto direction : directions) {
        const Engine::Point updatePoint = p + direction;
        if (outOfBound(updatePoint))
            continue;
        if (economyMap[updatePoint.y][updatePoint.x] != -1)
            economyMap[updatePoint.y][updatePoint.x] -= turretPriceState[p.y][p.x];
        if (mapDistance[p.y][p.x] != -1)
            economyMap[p.y][p.x] += turretPriceState[updatePoint.y][updatePoint.x];
    }
    // Update turretPriceState
    turretPriceState[p.y][p.x] = 0;
    // TODO:
    // After the current turret is removed, we should use BFS to iterate 'hollow' block
    // 'hollow' block is a block that is originally inside a wall of turrets
    // after appropriate mapDistance update, the 'hollow' block should have
    //  1. mapDistance != -1
    //  2. economyMap == -1
    //  ?. BFS should be enough for possible case...?
    return true;
}
void PlayScene::OnMouseDown(int button, int mx, int my) {
    if ((button & 1) && !imgTarget->Visible && preview) {
        // Cancel turret construct.
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
    IScene::OnMouseDown(button, mx, my);
}
void PlayScene::OnMouseMove(int mx, int my) {
    IScene::OnMouseMove(mx, my);
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (!preview || x < 0 || x >= MapWidth || y < 0 || y >= MapHeight) {
        imgTarget->Visible = false;
        return;
    }
    imgTarget->Visible = true;
    imgTarget->Position.x = x * BlockSize;
    imgTarget->Position.y = y * BlockSize;
}
void PlayScene::OnMouseUp(int button, int mx, int my) {
    IScene::OnMouseUp(button, mx, my);
    if (!imgTarget->Visible)
        return;
    const int x = mx / BlockSize;
    const int y = my / BlockSize;
    if (button & 1) {
        if (mapState[y][x] != TILE_OCCUPIED) {
            if (!preview)
                return;
            // Check if is specific tool
            // Check if valid
            if (dynamic_cast<ShovelTool*>(preview) || !CheckSpaceValid(x, y)) {
                Engine::Sprite* sprite;
                GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
                sprite->Rotation = 0;
                return;
            }
            // Remove Preview
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            // Construct real turret
            preview->Position.x = x * BlockSize + BlockSize / 2;
            preview->Position.y = y * BlockSize + BlockSize / 2;
            preview->Enabled = true;
            preview->Preview = false;
            preview->Tint = al_map_rgba(255, 255, 255, 255);
            mapState[y][x] = originalMapState[y][x];
            TryAddOneTurret(Engine::Point(x, y), preview);
            // To keep responding when paused
            preview->Update(0);
            // Remove Preview
            preview = nullptr;
            OnMouseMove(mx, my);
        } else {
            if (!preview)
                return;
            // Check if is specific tool
            // Check if is shovel-valid
            if (!dynamic_cast<ShovelTool*>(preview)) {
                Engine::Sprite* sprite;
                GroundEffectGroup->AddNewObject(sprite = new DirtyEffect("play/target-invalid.png", 1, x * BlockSize + BlockSize / 2, y * BlockSize + BlockSize / 2));
                sprite->Rotation = 0;
                return;
            }
            // Return money
            EarnMoney(turretPriceState[y][x] / 2);
            // Remove Preview
            preview->GetObjectIterator()->first = false;
            UIGroup->RemoveObject(preview->GetObjectIterator());
            TryRemoveOneTurret(Engine::Point(x, y));
            // To keep responding when paused
            preview->Update(0);
            // Remove Preview
            preview = nullptr;
            OnMouseMove(mx, my);
        }
    }
}
void PlayScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    else {
        keyStrokes.push_back(keyCode);
        if (keyStrokes.size() > code.size())
            keyStrokes.pop_front();
        if (keyCode == ALLEGRO_KEY_ENTER && keyStrokes.size() == code.size()) {
            auto it = keyStrokes.begin();
            for (int c : code) {
                if (!((*it == c) ||
                    (c == ALLEGRO_KEYMOD_SHIFT &&
                    (*it == ALLEGRO_KEY_LSHIFT || *it == ALLEGRO_KEY_RSHIFT))))
                    return;
                ++it;
            }
            this->maxMoney += 10000;
            this->EarnMoney(10000);
            EffectGroup->AddNewObject(new Plane());
        }
    }
    if (keyCode == ALLEGRO_KEY_Q) {
        // Hotkey for MachineGunTurret.
        UIBtnClicked(0);
    }
    else if (keyCode == ALLEGRO_KEY_W) {
        // Hotkey for LaserTurret.
        UIBtnClicked(1);
    }
    else if (keyCode == ALLEGRO_KEY_E) {
        // Hotkey for MissileTurret.
        UIBtnClicked(2);
    }
    else if (keyCode == ALLEGRO_KEY_R) {
        // Hotkey for HealingTurret.
        UIBtnClicked(3);
    }
    else if (keyCode == ALLEGRO_KEY_T) {
        // Hotkey for FlameThrowerTurret.
        UIBtnClicked(4);
    }
    else if (keyCode == ALLEGRO_KEY_X) {
        // Hotkey for ShovelTool.
        UIBtnClicked(5);
    }
    // TODO: [CUSTOM-TURRET]: Make specific key to create the turret.
    else if (keyCode >= ALLEGRO_KEY_0 && keyCode <= ALLEGRO_KEY_9) {
        // Hotkey for Speed up.
        SpeedMult = keyCode - ALLEGRO_KEY_0;
    }
}
void PlayScene::Hit() {
    lives--;
    UILives->Text = std::string("Life ") + std::to_string(lives);
    if (lives <= 0) {
        Engine::GameEngine::GetInstance().ChangeScene("lose-scene");
    }
}
int PlayScene::GetMoney() const {
    return money;
}
int PlayScene::GetMaxMoney() const {
    return maxMoney;
}
void PlayScene::EarnMoney(int money) {
    this->money += money;
    UIMoney->Text = std::string("$") + std::to_string(this->money);
}
int PlayScene::GetLives() const {
    return this->lives;
}
void PlayScene::ReadMap() {
    std::string filename = std::string("Resource/maps/") + std::to_string(MapId) + ".map";
    // Read map file.
    char c;
    std::vector<bool> mapData;
    std::ifstream fin(filename);
    while (fin >> c) {
        switch (c) {
        case '0': mapData.push_back(false); break;
        case '1': mapData.push_back(true); break;
        case '\n':
        case '\r':
            if (static_cast<int>(mapData.size()) / MapWidth != 0)
                throw std::ios_base::failure("Map data is corrupted.");
            break;
        default: throw std::ios_base::failure("Map data is corrupted.");
        }
    }
    fin.close();
    // Validate map data.
    if (static_cast<int>(mapData.size()) != MapWidth * MapHeight)
        throw std::ios_base::failure("Map data is corrupted.");
    // Store map in 2d array.
    mapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    originalMapState = std::vector<std::vector<TileType>>(MapHeight, std::vector<TileType>(MapWidth));
    for (int i = 0; i < MapHeight; i++) {
        for (int j = 0; j < MapWidth; j++) {
            const int num = mapData[i * MapWidth + j];
            mapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            originalMapState[i][j] = num ? TILE_FLOOR : TILE_DIRT;
            if (num)
                TileMapGroup->AddNewObject(new Engine::Image("play/floor.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
            else
                TileMapGroup->AddNewObject(new Engine::Image("play/dirt.png", j * BlockSize, i * BlockSize, BlockSize, BlockSize));
        }
    }
}
void PlayScene::InitializeMapAssociatedStates() {
    mapState.clear();
    ReadMap();
    // Initialize economyMap
    economyMap = std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth));
    mapDistance = CalculateBFSDistance();
    for (int i = 0; i < mapDistance.size(); i++)
        for (int j = 0; j < mapDistance[i].size(); j++)
            economyMap[i][j] = std::min(mapDistance[i][j], 0);
    // Initialize turrentState
    turretPriceState = std::vector<std::vector<int>>(MapHeight, std::vector<int>(MapWidth, 0));
}
void PlayScene::ReadEnemyWave() {
    // DONE: [HACKATHON-3-BUG] (3/5): Trace the code to know how the enemies are created.
    // DONE: [HACKATHON-3-BUG] (3/5): There is a bug in these files, which let the game only spawn the first enemy, try to fix it.
    std::string filename = std::string("Resource/enemies/") + std::to_string(MapId) + ".enm";
    // Read enemy file.
    int type;
    float wait, repeat;
    int maximunMoney = 0;
    enemyWaveData.clear();
    std::ifstream fin(filename);
    while (fin >> type && fin >> wait && fin >> repeat) {
        for (int i = 0; i < repeat; i++)
            enemyWaveData.emplace_back(type, wait);
        switch (type) {
        case 1:
            maximunMoney += SoldierEnemy::Reward * repeat;
            break;
        case 2:
            maximunMoney += PlaneEnemy::Reward * repeat;
            break;
        case 3:
            maximunMoney += TankEnemy::Reward * repeat;
            break;
        case 4:
            maximunMoney += ArmoredVehicleGreen::Reward * repeat;
            break;
        case 5:
            maximunMoney += CreeperEnemy::Reward * repeat;
            break;
        // TODO: [CUSTOM-ENEMY]: You need to modify 'Resource/enemy1.txt', or 'Resource/enemy2.txt' to spawn the 4th enemy.
        //         The format is "[EnemyId] [TimeDelay] [Repeat]".
        // TODO: [CUSTOM-ENEMY]: Enable the creation of the enemy.
        default:
            continue;
        }
    }
    fin.close();
    this->maxMoney = maximunMoney + PlayScene::InitialMoney;
}
void PlayScene::ConstructUI() {
    // Background
    UIGroup->AddNewObject(new Engine::Image("play/sand.png", 1280, 0, 320, 832));
    // Text
    UIGroup->AddNewObject(new Engine::Label(std::string("Stage ") + std::to_string(MapId), "pirulen.ttf", 32, 1294, 0));
    UIGroup->AddNewObject(UIMoney = new Engine::Label(std::string("$") + std::to_string(money), "pirulen.ttf", 24, 1294, 48));
    UIGroup->AddNewObject(UILives = new Engine::Label(std::string("Life ") + std::to_string(lives), "pirulen.ttf", 24, 1294, 88));
    TurretButton* btn;
    // Button 1
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tower-base.png", 1294, 136, 0, 0, 0, 0),
        Engine::Sprite("play/turret-1.png", 1294, 136 - 8, 0, 0, 0, 0)
        , 1294, 136, MachineGunTurret::Price);
    // Reference: Class Member Function Pointer and std::bind.
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 0));
    UIGroup->AddNewControlObject(btn);
    // Button 2
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tower-base.png", 1370, 136, 0, 0, 0, 0),
        Engine::Sprite("play/turret-2.png", 1370, 136 - 8, 0, 0, 0, 0)
        , 1370, 136, LaserTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 1));
    UIGroup->AddNewControlObject(btn);
    // Button 3
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tower-base.png", 1446, 136, 0, 0, 0, 0),
        Engine::Sprite("play/turret-3.png", 1446, 136, 0, 0, 0, 0)
        , 1446, 136, MissileTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 2));
    UIGroup->AddNewControlObject(btn);
    // Button 4
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tower-base.png", 1522, 136, 0, 0, 0, 0),
        Engine::Sprite("play/turret-8.png", 1522, 136 - 8, 0, 0, 0, 0)
        , 1522, 136, HealingTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 3));
    UIGroup->AddNewControlObject(btn);
    // Button 5
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tower-base.png", 1294, 212, 0, 0, 0, 0),
        Engine::Sprite("play/turret-6.png", 1294, 212 - 8, 0, 0, 0, 0)
        , 1294, 212, FlameThrowerTurret::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 4));
    UIGroup->AddNewControlObject(btn);
    // Button 6
    btn = new TurretButton("play/floor.png", "play/dirt.png",
        Engine::Sprite("play/tool-base.png", 1294, 288, 0, 0, 0, 0),
        Engine::Sprite("play/shovel.png", 1294 + 32, 288 + 32, 0, 0, .5, .5, ALLEGRO_PI / 4)
        , 1294, 288, ShovelTool::Price);
    btn->SetOnClickCallback(std::bind(&PlayScene::UIBtnClicked, this, 5));
    UIGroup->AddNewControlObject(btn);
    // TODO: [CUSTOM-TURRET]: Create a button to support constructing the turret.
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int shift = 135 + 25;
    dangerIndicator = new Engine::Sprite("play/benjamin.png", w - shift, h - shift);
    dangerIndicator->Tint.a = 0;
    UIGroup->AddNewObject(dangerIndicator);
}

void PlayScene::UIBtnClicked(int id) {
    if (preview) {
        UIGroup->RemoveObject(preview->GetObjectIterator());
        preview = nullptr;
    }
    // TODO: [CUSTOM-TURRET]: On callback, create the turret.
    if (id == 0 && money >= MachineGunTurret::Price)
        preview = new MachineGunTurret(0, 0);
    else if (id == 1 && money >= LaserTurret::Price)
        preview = new LaserTurret(0, 0);
    else if (id == 2 && money >= MissileTurret::Price)
        preview = new MissileTurret(0, 0);
    else if (id == 3 && money >= HealingTurret::Price)
        preview = new HealingTurret(0, 0);
    else if (id == 4 && money >= FlameThrowerTurret::Price)
        preview = new FlameThrowerTurret(0, 0);
    else if (id == 5 && money >= ShovelTool::Price)
        preview = new ShovelTool(0, 0);
    if (!preview)
        return;
    preview->Position = Engine::GameEngine::GetInstance().GetMousePosition();
    preview->Tint = al_map_rgba(255, 255, 255, 200);
    preview->Enabled = false;
    preview->Preview = true;
    UIGroup->AddNewObject(preview);
    OnMouseMove(Engine::GameEngine::GetInstance().GetMousePosition().x, Engine::GameEngine::GetInstance().GetMousePosition().y);
}

bool PlayScene::CheckSpaceValid(int x, int y) {
    if (x < 0 || x >= MapWidth || y < 0 || y >= MapHeight)
        return false;
    PlayScene::TileType map00 = mapState[y][x];
    mapState[y][x] = TILE_OCCUPIED;
    std::vector<std::vector<int>> map = CalculateBFSDistance();
    mapState[y][x] = map00;
    if (map[0][0] == -1)
        return false;
    for (auto& it : EnemyGroup->GetObjects()) {
        Engine::Point pnt;
        pnt.x = floor(it->Position.x / BlockSize);
        pnt.y = floor(it->Position.y / BlockSize);
        if (pnt.x < 0) pnt.x = 0;
        if (pnt.x >= MapWidth) pnt.x = MapWidth - 1;
        if (pnt.y < 0) pnt.y = 0;
        if (pnt.y >= MapHeight) pnt.y = MapHeight - 1;
        if (map[pnt.y][pnt.x] == -1)
            return false;
    }
    // All enemy have path to exit.
    mapState[y][x] = TILE_OCCUPIED;
    mapDistance = map;
    for (auto& it : EnemyGroup->GetObjects())
        dynamic_cast<Enemy*>(it)->UpdatePath(mapDistance);
    return true;
}
std::vector<std::vector<int>> PlayScene::CalculateBFSDistance() {
    // Reverse BFS to find path.
    std::vector<std::vector<int>> map(MapHeight, std::vector<int>(std::vector<int>(MapWidth, -1)));
    std::queue<Engine::Point> que;
    // Push end point.
    // BFS from end point.
    if (mapState[MapHeight - 1][MapWidth - 1] != TILE_DIRT)
        return map;
    que.push(Engine::Point(MapWidth - 1, MapHeight - 1));
    map[MapHeight - 1][MapWidth - 1] = 0;
    auto outside_the_map = [this] (int x, int y) -> bool {
        return x < 0
            || x >= this->MapWidth
            || y < 0
            || y >= this->MapHeight;
    };
    while (!que.empty()) {
        Engine::Point p = que.front();
        que.pop();
        // DONE: [BFS PathFinding] (1/1): Implement a BFS starting from the most right-bottom block in the map.
        //               For each step you should assign the corresponding distance to the most right-bottom block.
        //               mapState[y][x] is TILE_DIRT if it is empty.
        float direction[4][2] = {
            {1, 0},
            {-1, 0},
            {0, 1},
            {0, -1}
        };
        for (
            int i = 0, dx = direction[0][0], dy = direction[0][1];
            i < 4;
            i++, dx = direction[i][0], dy = direction[i][1]
        ) {
            int x = p.x + dx;
            int y = p.y + dy;
            if (outside_the_map(x, y) || this->mapState[y][x] != TILE_DIRT || map[y][x] != -1)
                continue;
            que.push(Engine::Point((float)x, (float)y));
            map[y][x] = map[p.y][p.x] + 1;
        }
    }
    return map;
}
