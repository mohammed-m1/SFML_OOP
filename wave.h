#ifndef WAVE_H
#define WAVE_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "enemy.h"
#include "skeleton.h"
#include "goblin.h"
#include "ogre.h"

struct Wave {
    int skeletons;
    int goblins;
    int ogres;
};

class WaveManager {
private:
    std::vector<Wave> waves;
    int   currentWaveIndex = 0;
    float enemySpeed;

public:
    WaveManager(const std::vector<Wave>& waves_, float enemySpeed_);

    bool hasNextWave() const;

    //  spawn function for waves
    void spawnNextWave(std::vector<enemy>& enemies,
                       const std::vector<sf::Vector2i>& pathCells,
                       const sf::Vector2f& offset);
};

#endif
