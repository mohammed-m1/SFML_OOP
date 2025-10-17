#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

int main() {
    // 1000 x 1000 window (main display)
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{1000, 1000}), "SFML Grid Centered");
    // window.setFramerateLimit(60);

    // 🟨 [2] DEFINE THE INNER GRID SCREEN SIZE (800×600)
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    // 🟩 [3] CENTER THE GRID INSIDE THE 1000×1000 WINDOW
    const sf::Vector2f offset{
        (1000.0f - screenWidth) / 2.0f,   // X offset
        (1000.0f - screenHeight) / 2.0f   // Y offset
    };

    // 🟨 [4] GRID SETTINGS
    const float gridSize = 50.0f;
    const int gridWidth = static_cast<int>(screenWidth / gridSize);
    const int gridHeight = static_cast<int>(screenHeight / gridSize);

    // 🟨 [5] CREATE THE 800×600 GRID (DRAWN INSIDE THE WINDOW)
    sf::VertexArray grid(sf::PrimitiveType::Lines);

    // 🔹 Vertical lines
    for (int x = 0; x <= gridWidth; ++x) {
        float xpos = static_cast<float>(x) * gridSize + offset.x;
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y + screenHeight}, sf::Color::Green });
    }

    // 🔹 Horizontal lines
    for (int y = 0; y <= gridHeight; ++y) {
        float ypos = static_cast<float>(y) * gridSize + offset.y;
        grid.append(sf::Vertex{ sf::Vector2f{offset.x, ypos}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{offset.x + screenWidth, ypos}, sf::Color::Green });
    }

    // 🟪 [6] MOVING CYAN CIRCLE
    sf::CircleShape circle(20.0f);
    circle.setFillColor(sf::Color::Cyan);
    circle.setOrigin(sf::Vector2f{circle.getRadius(), circle.getRadius()});
    circle.setPosition(sf::Vector2f{offset.x + screenWidth - 50.0f, offset.y + screenHeight / 2.0f});
    float circleSpeed = -200.0f; // Move left

    // 🟪 [7] COLOR SELECTION RECTANGLES
    sf::RectangleShape brownRect(sf::Vector2f{50.0f, 50.0f});
    brownRect.setFillColor(sf::Color(139, 69, 19)); // Brown
    brownRect.setPosition(sf::Vector2f{offset.x + 50.0f, offset.y + screenHeight + 50.0f});
    brownRect.setOutlineColor(sf::Color::Transparent);
    brownRect.setOutlineThickness(2.0f);

    sf::RectangleShape purpleRect(sf::Vector2f{50.0f, 50.0f});
    purpleRect.setFillColor(sf::Color(128, 0, 128)); // Purple
    purpleRect.setPosition(sf::Vector2f{offset.x + 110.0f, offset.y + screenHeight + 50.0f});
    purpleRect.setOutlineColor(sf::Color::Transparent);
    purpleRect.setOutlineThickness(2.0f);

    // 🟪 [8] PLACED CIRCLES STORAGE AND COLOR SELECTION
    std::vector<sf::CircleShape> placedCircles;
    sf::Color selectedColor = sf::Color(128, 0, 128); // Default to purple
    bool colorSelected = false; // Tracks if a color has been selected

    // Clock for delta time
    sf::Clock clock;

    while (window.isOpen()) {
        // Process events
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseEvent = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    sf::Vector2f pos(static_cast<float>(mouseEvent->position.x), static_cast<float>(mouseEvent->position.y));

                    // Check if clicking on brown rectangle
                    if (brownRect.getGlobalBounds().contains(pos)) {
                        selectedColor = sf::Color(139, 69, 19); // Select brown
                        colorSelected = true;
                        brownRect.setOutlineColor(sf::Color::White);
                        purpleRect.setOutlineColor(sf::Color::Transparent);
                    }
                    // Check if clicking on purple rectangle
                    else if (purpleRect.getGlobalBounds().contains(pos)) {
                        selectedColor = sf::Color(128, 0, 128); // Select purple
                        colorSelected = true;
                        purpleRect.setOutlineColor(sf::Color::White);
                        brownRect.setOutlineColor(sf::Color::Transparent);
                    }
                    // Check if clicking within the grid to place a circle
                    else if (colorSelected && pos.x >= offset.x && pos.x <= offset.x + screenWidth &&
                             pos.y >= offset.y && pos.y <= offset.y + screenHeight) {
                        if (placedCircles.size() < 3) {
                            sf::CircleShape newCircle(10.0f);
                            newCircle.setFillColor(selectedColor);
                            newCircle.setOrigin(sf::Vector2f{10.0f, 10.0f});
                            newCircle.setPosition(pos);
                            placedCircles.push_back(newCircle);
                            colorSelected = false; // Reset color selection
                            brownRect.setOutlineColor(sf::Color::Transparent);
                            purpleRect.setOutlineColor(sf::Color::Transparent);
                        }
                    }
                }
            }
        }

        // Update cyan circle position
        float deltaTime = clock.restart().asSeconds();
        circle.move(sf::Vector2f{circleSpeed * deltaTime, 0.0f});

        // Reset cyan circle if it goes off the left side of the grid
        if (circle.getPosition().x < offset.x - circle.getRadius() * 2.0f) {
            circle.setPosition(sf::Vector2f{offset.x + screenWidth + circle.getRadius() * 2.0f, offset.y + screenHeight / 2.0f});
        }

        window.clear(sf::Color::Black);
        window.draw(grid);     // 🟨 draws the 800×600 grid
        window.draw(circle);   // 🟪 draws the cyan circle
        for (const auto& p : placedCircles) {
            window.draw(p);    // 🟪 draws placed circles
        }
        window.draw(brownRect); // 🟫 draws the brown rectangle
        window.draw(purpleRect); // 🟪 draws the purple rectangle

        window.display();
    }

    return 0;
}