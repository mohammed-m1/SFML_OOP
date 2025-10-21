#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <SFML/Graphics.hpp>
    class enemy
    {
    protected:
        int damage;
        int health;
        std::string res;
        int quantity;
        sf::Vector2f position;
        sf::CircleShape shape;
        std::vector<sf::Vector2i> path; // path for the enemy
        int currentPathIndex;            // which cell the enemy is moving toward
        float speed;
        float spawnDelay = 0.f; // delay before this enemy starts moving
        float spawnTimer = 0.f;
    public:

        enemy();

        enemy(int quantity, int damage, int health, std::string res);

        enemy(int quantity, int damage, int health, std::string res, const std::vector<sf::Vector2i>& pathCells);

        void take_damage(int damage, std::string dam_type);

        int get_damage();

        int get_health();

        void set_health(int health);

        void set_speed(int spd);

        std::string get_res();

        bool isAlive() const;

        void set_position(const sf::Vector2f& pos);

        void set_path(const std::vector<sf::Vector2i> pathcell);

        void set_spawnDelay(float delay);
    
        sf::Vector2f get_position() const;

        sf::CircleShape& get_shape();

        bool update(float dt, float gridSize = 50.f, const sf::Vector2f& offset = {100.f, 100.f});
        // virtual ~enemy();
    };
    
#endif