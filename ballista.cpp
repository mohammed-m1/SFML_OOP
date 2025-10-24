#include "ballista.h"

    ballista::ballista(int location) : building(5, 5, 3, "physical") {
        this->location = location;
        cost = 100;
        color = sf::Color(139, 69, 19); // brown
        fireInterval = 1.0f / fire_rate;
        lastShot = 0.f;
    }

    void ballista::upgrade() {
        if (level == 1) {
            set_fire_rate(get_fire_rate()*1.5);
            set_damage(get_damage()*1.5);

            level++;
        } else {
            set_fire_rate(get_fire_rate()*1.25);
            set_damage(get_damage()*1.25);
            level++;
        }        
    }

    int ballista:: get_location() {
        return location;
    }

    // void ballista::upgrade() {
    //     set_damage(get_damage() * 1.5f);
    //     level++;
    // } 
   