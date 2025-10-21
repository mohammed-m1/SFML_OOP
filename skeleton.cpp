#include "skeleton.h"
#include <SFML/Graphics.hpp>

skeleton::skeleton(int quantity) : enemy(quantity, 2, 20, "magical") {
    this->quantity = quantity;
    shape.setFillColor(sf::Color::White); // 
}

// int skeleton::get_quantity() {
//     return quantity;
// }
