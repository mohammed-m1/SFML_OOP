#include "player.h"

    player::player() {}

    player::player(int difficulty) {
        this->difficulty = difficulty;
        this->health = (10 - 2*(this->difficulty-1));
        this->money = (500);
    }

     int player::get_health() const {
        return health;
    }

    int player::get_money() const{
        return money;
    }

    int player::get_difficulty()const {
        return difficulty;
    }
    
    void player::add_money(int add_money) {
        this->money = this->money + add_money;
    }

    void player::add_health(int add_health) {
        this->health = this->health + add_health;
    }

    //player::~player() {}

    // int player::get_waveNumber() {
    //     return waveNumber;
    // }

    // int player::get_multiplier() {
    //     return multiplier;
    // }

    // void player::set_health(int health) {
    //     this->health = health;
    // }

    // void player::next_wave() {
    //     waveNumber++;
    // }