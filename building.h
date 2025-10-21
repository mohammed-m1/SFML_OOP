#ifndef BUILDING_H
#define BUILDING_H

// #include "player.h"
#include <string>
#include <SFML/Graphics.hpp>


class building
{
protected:
    float damage; //float for multipliers
    int range;
    float fire_rate;
    std::string damageType;  // std::string priority;  for if we get to it
    int level;

    // Added for gameplay integration:
    float fireInterval;   // derived from fire_rate (seconds between shots)
    float lastShot;       // time since last shot
    int cost;
    sf::Color color;  //color
    sf::Vector2f pos;  //location of tower xy

public:
    building();

    building(float damage, int range, float fire_rate, std::string damageType);

    virtual ~building() = default;

    float get_damage();

    int get_range();

    float get_fire_rate();

    int get_level();

    std::string get_damageType();

    void set_damage(float damage);

    void set_fire_rate(float fire_rate);

    // void costCalc(int money);

    void set_position(sf::Vector2f p);

    sf::Vector2f get_position() const;

    void set_color(sf::Color c);

    sf::Color get_color() const;

    void set_cost(int c);

    int get_cost() const;

    // void set_priority(std::string priority);

    virtual void upgrade();

    // virtual void attack(int enemy);
    void reset_shot_timer();

    void update_shot_timer(float dt); //if it shoots then we need to fin trime delay for next shot then
    
    bool ready_to_fire() const; //true if it is


};

#endif