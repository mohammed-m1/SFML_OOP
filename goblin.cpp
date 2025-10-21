#include "goblin.h"
#include <SFML/Graphics.hpp>


goblin::goblin(int quantity) : enemy(quantity, 2, 30, "none") {
    this->quantity = quantity;
    shape.setFillColor(sf::Color::Green);
}

// int goblin::get_quantity() {
//     return quantity;
// }
