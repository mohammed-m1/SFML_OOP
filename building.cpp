#include "building.h"
    
    building::building(){}

    building::building(float damage, int range, float fire_rate, std::string damageType) {
        this->damage = damage;
        this->range = range;
        this->fire_rate = fire_rate;
        this->level = 1;
        this->damageType = damageType;
    }

    // building::~building() {
    // }


    float building::get_damage() {
        return damage;
    }

    int building::get_range() {
        return range;
    }

    float building::get_fire_rate() {
        return fire_rate;
    }

    int building::get_level() {
        return level;
    }

    std::string building::get_damageType() {
        return damageType;
    }


    void building::set_damage(float damage) {
        this->damage = damage;
    }

    void building::set_fire_rate(float fire_rate) {
        this->fire_rate = fire_rate;
    }

    void building::upgrade() {}
    
    // void cost(int money);

    // void building::attack(int enemy) {}

    sf::Vector2f building::get_position() const{
        return pos;
    };

    
    void building::set_position(sf::Vector2f p){
        pos = p;
    };



    void building::set_color(sf::Color c){
        color = c;
    };

    sf::Color building::get_color() const{
        return color;
    };

    void building::set_cost(int c){
        cost = c;
    };

    int building::get_cost() const{
        return cost;
    };

    // void set_priority(std::string priority);

    // virtual void building::upgrade(){

    // };

    // virtual void attack(int enemy);
    void building::reset_shot_timer(){
        lastShot = 0.f;
    };

    void building::update_shot_timer(float dt){
        lastShot = lastShot + dt;
    }; //if it shoots then we need to fin trime delay for next shot then
    
    bool building::ready_to_fire() const{
        return lastShot >= fireInterval;
    }; //true if it is

    // virtual void upgrade() = 0; // must be implemented by children

    // virtual building::~building(){
        
    // }
