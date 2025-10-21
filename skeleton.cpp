#include "skeleton.h"
#include <SFML/Graphics.hpp>

skeleton::skeleton(int quantity) : enemy(quantity, 1, 70, "magical") {
    this->quantity = quantity;
    shape.setFillColor(sf::Color::White); // 
}

// int skeleton::get_quantity() {
//     return quantity;
// }
