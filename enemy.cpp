#include "enemy.h"
#include <cmath>

sf::Vector2f cellToPixel(const sf::Vector2i& cell, float gridSize = 50.f, const sf::Vector2f& offset = {100.f, 100.f});

enemy::enemy(int quantity, int damage, int health, std::string res) {
    this->damage = damage;
    this->health = health;
    this->res = res;
    this->quantity = quantity;
    this->shape = sf::CircleShape(10.f);
    this->shape.setOrigin({10.f, 10.f});
    this->currentPathIndex = 0;
    // set_position(position);
    health = 100;

}

enemy::enemy(int quantity, int damage, int health, std::string res, const std::vector<sf::Vector2i>& pathCells) {
    this->damage = damage;
    this->health = health;
    this->res = res;
    this->quantity = quantity;
    this->path = pathCells;
    this->shape = sf::CircleShape(10.f);
    this->shape.setOrigin({10.f, 10.f});
    this->currentPathIndex = 0;

    // Start position = first cell of the path
    position = cellToPixel(path.front(), 50, {100.f, 100.f});
    set_position(position);
}

enemy::enemy(){
    this->damage = 0;
    this->health = 0;
    this->res = "none";
    this->quantity = 0;   
}

void enemy::take_damage(int damage, std::string dam_type) {
    if (res == dam_type)
    {
        health = health - 0.5*damage;
    } else {
        health = health - damage;
    }
}

int enemy::get_damage() {
    return damage;
}

int enemy::get_health() {
    return health;
}

std::string enemy::get_res() {
    return res;
}

bool enemy::isAlive() const {
    return health > 0;
}

void enemy::set_position(const sf::Vector2f& pos) {
    position = pos;
    shape.setPosition(pos);
}

sf::Vector2f enemy::get_position() const {
    return position;
}

sf::CircleShape& enemy::get_shape() {
    return shape;
}

sf::Vector2f cellToPixel(const sf::Vector2i& cell, float gridSize, const sf::Vector2f& offset) {
    return { offset.x + cell.x * gridSize + gridSize * 0.5f,
             offset.y + cell.y * gridSize + gridSize * 0.5f };
}

void enemy::set_spawnDelay(float delay){
    this->spawnDelay = delay;
};


void enemy::update(float dt, float gridSize, const sf::Vector2f& offset) {
   if (spawnTimer < spawnDelay) {
        spawnTimer += dt;
        return;
    }


    if (currentPathIndex >= static_cast<int>(path.size()) - 1) {
        health = 0;
        return;
    }

    // Target the next path cell
    sf::Vector2f targetPos = cellToPixel(path[currentPathIndex + 1], gridSize, offset);
    sf::Vector2f dir = targetPos - position;

    float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (dist < 0.5f) {
        currentPathIndex++;
        if (currentPathIndex >= static_cast<int>(path.size()) - 1) {
            health = 0;
        }
        return;
    }

    dir /= dist;  // normalize direction
    set_position(position + dir * speed * dt);
}


void enemy::set_speed(int spd){
    this->speed = spd;
}

void enemy::set_path(const std::vector<sf::Vector2i> pathcell){
    path = pathcell;
    currentPathIndex = 0;
    // Use same grid/offset as main (50.f, {100,100}) – or pass in if you prefer.
    const float gridSize = 50.f;
    const sf::Vector2f offset{100.f, 100.f};
    set_position(cellToPixel(path.front(), gridSize, offset));
}