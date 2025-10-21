#include "wave.h"
#include "enemy.h"
#include "skeleton.h"
#include "goblin.h"
#include "ogre.h"


WaveManager::WaveManager(const std::vector<Wave>& waves_, float enemySpeed_) 
    : waves(waves_), enemySpeed(enemySpeed_) {}

bool WaveManager::hasNextWave() const {
    return currentWaveIndex < waves.size();
}

void WaveManager::spawnNextWave(std::vector<enemy>& enemies, const std::vector<sf::Vector2i>& pathCells) {
    if (!hasNextWave()) return;

    const sf::Vector2f offset((1000 - 800) / 2, (1000 - 600) / 2);
    auto cellToPixel = [&](const sf::Vector2i& cell)->sf::Vector2f {
        return { offset.x + cell.x * 50 + 25.f, offset.y + cell.y * 50 + 25.f };
    };

    const Wave& wave = waves[currentWaveIndex];

    // Delay between individual enemy spawns
    const float delayBetweenEnemies = 0.5f; // half a second per enemy
    float currentDelay = 0.f;

    auto spawnEnemyWithDelay = [&](auto&& createEnemy, int count) {
        for (int i = 0; i < count; ++i) {
            auto e = createEnemy(1);
            e.set_path(pathCells);
            e.set_speed(enemySpeed);
            e.set_position(cellToPixel(pathCells[0]));
            e.set_spawnDelay(currentDelay);  // we'll use this field to delay movement
            enemies.push_back(e);
            currentDelay += delayBetweenEnemies;
        }
    };

    spawnEnemyWithDelay([](int q){ return skeleton(q); }, wave.skeletons);
    spawnEnemyWithDelay([](int q){ return goblin(q); }, wave.goblins);
    spawnEnemyWithDelay([](int q){ return ogre(q); }, wave.ogres);

    currentWaveIndex++;
}