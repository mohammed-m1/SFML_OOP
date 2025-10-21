#include "wave.h"
#include "enemy.h"
#include "skeleton.h"
#include "goblin.h"
#include "ogre.h"
#include <cmath>


WaveManager::WaveManager(const std::vector<Wave>& waves_, float enemySpeed_) 
    : waves(waves_), enemySpeed(enemySpeed_) {}

bool WaveManager::hasNextWave() const {
    return currentWaveIndex < waves.size();
}

void WaveManager::spawnNextWave(std::vector<enemy>& enemies, const std::vector<sf::Vector2i>& pathCells) {
    if (!hasNextWave()) return;

    const sf::Vector2f offset((1000 - 800) / 2.f, (1000 - 600) / 2.f);
    auto cellToPixel = [&](const sf::Vector2i& cell)->sf::Vector2f {
        return { offset.x + cell.x * 50 + 25.f, offset.y + cell.y * 50 + 25.f };
    };

    const Wave& wave = waves[currentWaveIndex];

    const float spacing = 40.f; // distance between enemies
    float startOffset = 0.f;    // accumulates spacing along path

    auto spawnEnemyType = [&](auto createEnemy, int count) {
        for (int i = 0; i < count; ++i) {
            auto e = createEnemy(1);
            e.set_path(pathCells);
            e.set_speed(enemySpeed);

            // Base position at start of path
            sf::Vector2f base = cellToPixel(pathCells[0]);

            // Direction of first segment
            sf::Vector2f dir = {0.f,0.f};
            if (pathCells.size() > 1) {
                sf::Vector2f next = cellToPixel(pathCells[1]);
                dir = next - base;
                float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
                if (len > 0) dir /= len;
            }

            // Shift backward along path by current offset
            sf::Vector2f pos = base - dir * startOffset;
            e.set_position(pos);

            enemies.push_back(e);
            startOffset += spacing; // increment spacing for next enemy and see if it worke
        }
    };

    spawnEnemyType([](int q){ return skeleton(q); }, wave.skeletons);
    spawnEnemyType([](int q){ return goblin(q); }, wave.goblins);
    spawnEnemyType([](int q){ return ogre(q); }, wave.ogres);

    currentWaveIndex++;
}
