#include "mage_tower.h"

    mage_tower::mage_tower(int location) : building(4, 4, 6, "magic") {
        // add_money(-150);
        this->location = location;
        this->cost = 150;
        this->color = sf::Color(0, 100, 0);
        this->fireInterval = 1.0f / fire_rate;
        this->lastShot = 0.f;
    }


    void mage_tower::upgrade() {
        // add_money(-150*get_level());
        set_damage(get_damage()*1.5);
        level++;
    }

    int mage_tower:: get_location() {
        return location;
    }

    // void mage_tower::attack(int enemy) {
        //need enemy to deal damage
    // }

   