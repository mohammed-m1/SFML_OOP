#include "ogre.h"
#include <SFML/Graphics.hpp>

ogre::ogre(int quantity) : enemy(quantity, 5, 50, "physical") {
    this->quantity = quantity;
    shape.setFillColor(sf::Color(128, 64, 0)); // brownish
}

// int ogre::get_quantity() {
//     return quantity;
// }
