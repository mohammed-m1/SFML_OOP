#include "wave.h"
#include <SFML/System/Vector2.hpp>

WaveManager::WaveManager(const std::vector<Wave>& waves_, float enemySpeed_)
    : waves(waves_), enemySpeed(enemySpeed_) {}

bool WaveManager::hasNextWave() const {
    return currentWaveIndex < static_cast<int>(waves.size());
}

void WaveManager::spawnNextWave(std::vector<enemy>& enemies,
                                const std::vector<sf::Vector2i>& pathCells,
                                const sf::Vector2f& offset)
{
    if (!hasNextWave() || pathCells.empty()) return;

    const float gridSize = 50.f;

    auto cellToPixel = [&](const sf::Vector2i& cell) -> sf::Vector2f {
        return {
            offset.x + cell.x * gridSize + gridSize * 0.5f,
            offset.y + cell.y * gridSize + gridSize * 0.5f
        };
    };

    const Wave& wave = waves[currentWaveIndex];

    const float delayStep = 0.7f; // seconds between enemies
    float nextDelay = 0.f;

    auto spawnEnemyType = [&](auto makeEnemy, int count) {
        for (int i = 0; i < count; ++i) {
            auto e = makeEnemy(1);
            e.set_path(pathCells);
            e.set_speed(enemySpeed);
            e.set_position(cellToPixel(pathCells.front())); // center of first tile
            e.set_spawnDelay(nextDelay);                    // stagger in time
            enemies.push_back(e);
            nextDelay += delayStep;
        }
    };

    spawnEnemyType([](int q){ return skeleton(q); }, wave.skeletons);
    spawnEnemyType([](int q){ return goblin(q);   }, wave.goblins);
    spawnEnemyType([](int q){ return ogre(q);     }, wave.ogres);

    currentWaveIndex++;
}
