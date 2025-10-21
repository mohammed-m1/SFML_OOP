#include "farm.h"

    farm::farm(int location) : building(0, 0, 0, "physical") {
        this->location = location;
        farmRate = 60; //per 5sec
        cost = 100;
        color = sf::Color(0, 100, 0); // green
    }

    void farm::upgrade() {
        farmRate = farmRate + 100;
    }

    int farm::get_location() {
        return location;
    }

    int farm::get_farmRate() {
        return farmRate;
    }


   