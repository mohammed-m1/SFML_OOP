#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

sf::CircleShape circleMoveCyan(const sf::Vector2f offset, const float screenWidth, const float screenHeight) {
    // Cyan circle moving from right to left
    sf::CircleShape circle(20.0f);
    circle.setFillColor(sf::Color::Cyan);
    circle.setOrigin(sf::Vector2f{circle.getRadius(), circle.getRadius()});
    circle.setPosition(sf::Vector2f(offset.x + screenWidth - 50.0f, offset.y + screenHeight / 2.0f)); // Start on right
    return circle;
}

sf::VertexArray makeDisplay(float screenWidth, float screenHeight, const int gridWidth, const float gridSize, const int gridHeight, const sf::Vector2f offset, sf::VertexArray grid) {
    // Vertical lines making
    for (int x = 0; x <= gridWidth; ++x) {
        float xpos = static_cast<float>(x) * gridSize + offset.x;
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y + screenHeight}, sf::Color::Green });
    }

    // Horizontal lines making
    for (int y = 0; y <= gridHeight; ++y) {
        float ypos = static_cast<float>(y) * gridSize + offset.y;
        grid.append(sf::Vertex{ sf::Vector2f{offset.x, ypos}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{offset.x + screenWidth, ypos}, sf::Color::Green });
    }
    return grid;
}

int main() {
    // 1000 x 1000 window (main display)
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{1000, 1000}), "SFML Grid Centered");
    window.setFramerateLimit(60);

    // Load font for text
    sf::Font font;
if (!font.openFromFile("C:\\Windows\\Fonts\\ARIAL.ttf")) {
    return 1; // Exit if font fails to load
}

    // Inner screen grid of size 800 x 600
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    // Centre the screen grid inside the window
    const sf::Vector2f offset{
        (1000.0f - screenWidth) / 2.0f,   // X offset
        (1000.0f - screenHeight) / 2.0f   // Y offset
    };

    // Grid size and spacing and how many are there
    const float gridSize = 50.0f;
    const int gridWidth = static_cast<int>(screenWidth / gridSize); // Number of cols for array
    const int gridHeight = static_cast<int>(screenHeight / gridSize); // Number of rows for array

    // Creating the 800 x 600 screen by displaying it
    sf::VertexArray grid(sf::PrimitiveType::Lines);

    grid = makeDisplay(screenWidth, screenHeight, gridWidth, gridSize, gridHeight, offset, grid); // Makes the lines for grid.

    sf::CircleShape circle = circleMoveCyan(offset, screenWidth, screenHeight); // Make circle

    // Selecting towers and the place
    sf::RectangleShape brownRect;
    brownRect.setSize(sf::Vector2f{60.0f, 60.0f});
    brownRect.setFillColor(sf::Color(139, 69, 19)); // Brown
    brownRect.setPosition(sf::Vector2f{offset.x + 60.0f, offset.y + screenHeight + 60.0f});
    brownRect.setOutlineColor(sf::Color::Transparent);
    brownRect.setOutlineThickness(2.0f);

    sf::RectangleShape greenRect;
    greenRect.setSize(sf::Vector2f{60.0f, 60.0f});
    greenRect.setFillColor(sf::Color(0, 100, 0)); // Royal green
    greenRect.setPosition(sf::Vector2f{offset.x + 130.0f, offset.y + screenHeight + 60.0f}); // Adjusted for gap
    greenRect.setOutlineColor(sf::Color::Transparent);
    greenRect.setOutlineThickness(2.0f);

    sf::RectangleShape purpleRect;
    purpleRect.setSize(sf::Vector2f{60.0f, 60.0f});
    purpleRect.setFillColor(sf::Color(128, 0, 128)); // Purple
    purpleRect.setPosition(sf::Vector2f{offset.x + 200.0f, offset.y + screenHeight + 60.0f}); // Adjusted for gap
    purpleRect.setOutlineColor(sf::Color::Transparent);
    purpleRect.setOutlineThickness(2.0f);

    // Text for rectangles
    sf::Text brownText(font, "Ballista", 20);
    brownText.setFillColor(sf::Color::White);
    sf::FloatRect brownBounds = brownText.getLocalBounds();
    brownText.setPosition(sf::Vector2f(brownRect.getPosition().x + (brownRect.getSize().x - brownBounds.size.x) / 2.0f,
                          brownRect.getPosition().y + (brownRect.getSize().y - brownBounds.size.y) / 2.0f - brownBounds.position.y / 2.0f));

    sf::Text greenText(font, "Mage", 20);
    greenText.setFillColor(sf::Color::White);
    sf::FloatRect greenBounds = greenText.getLocalBounds();
    greenText.setPosition(sf::Vector2f(greenRect.getPosition().x + (greenRect.getSize().x - greenBounds.size.x) / 2.0f,
                          greenRect.getPosition().y + (greenRect.getSize().y - greenBounds.size.y) / 2.0f - greenBounds.position.y / 2.0f));

    sf::Text purpleText(font, "Farm", 20);
    purpleText.setFillColor(sf::Color::White);
    sf::FloatRect purpleBounds = purpleText.getLocalBounds();
    purpleText.setPosition(sf::Vector2f(purpleRect.getPosition().x + (purpleRect.getSize().x - purpleBounds.size.x) / 2.0f,
                           purpleRect.getPosition().y + (purpleRect.getSize().y - purpleBounds.size.y) / 2.0f - purpleBounds.position.y / 2.0f));

    // PLACED CIRCLES STORAGE AND COLOR SELECTION
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

                    // Reset outlines
                    brownRect.setOutlineColor(sf::Color::Transparent);
                    greenRect.setOutlineColor(sf::Color::Transparent);
                    purpleRect.setOutlineColor(sf::Color::Transparent);

                    // Check if clicking on brown rectangle
                    if (brownRect.getGlobalBounds().contains(pos)) {
                        selectedColor = sf::Color(139, 69, 19); // Select brown
                        colorSelected = true;
                        brownRect.setOutlineColor(sf::Color::White);
                    }
                    // Check if clicking on green rectangle
                    else if (greenRect.getGlobalBounds().contains(pos)) {
                        selectedColor = sf::Color(0, 100, 0); // Select green
                        colorSelected = true;
                        greenRect.setOutlineColor(sf::Color::White);
                    }
                    // Check if clicking on purple rectangle
                    else if (purpleRect.getGlobalBounds().contains(pos)) {
                        selectedColor = sf::Color(128, 0, 128); // Select purple
                        colorSelected = true;
                        purpleRect.setOutlineColor(sf::Color::White);
                    }
                    // Check if clicking within the grid to place a circle
                    else if (colorSelected && pos.x >= offset.x && pos.x <= offset.x + screenWidth &&
                             pos.y >= offset.y && pos.y <= offset.y + screenHeight) {
                        if (placedCircles.size() < 3) { // this is where the less than 3 check happens.
                            // Calculate grid cell index
                            int cellX = static_cast<int>((pos.x - offset.x) / gridSize);
                            int cellY = static_cast<int>((pos.y - offset.y) / gridSize);
                            // Calculate center of the cell
                            float centerX = offset.x + static_cast<float>(cellX) * gridSize + gridSize / 2.0f;
                            float centerY = offset.y + static_cast<float>(cellY) * gridSize + gridSize / 2.0f;
                            // Create and position circle at center
                            sf::CircleShape newCircle(10.0f);
                            newCircle.setFillColor(selectedColor);
                            newCircle.setOrigin(sf::Vector2f{10.0f, 10.0f});
                            newCircle.setPosition(sf::Vector2f{centerX, centerY});
                            placedCircles.push_back(newCircle);
                            colorSelected = false; // Reset color selection
                            brownRect.setOutlineColor(sf::Color::Transparent);
                            greenRect.setOutlineColor(sf::Color::Transparent);
                            purpleRect.setOutlineColor(sf::Color::Transparent);
                        }
                    }
                }
            }
        }

        // Update cyan circle position
        float deltaTime = clock.restart().asSeconds();
        float circleSpeed = -200.0f; // Move left
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
        window.draw(greenRect); // 🟫 draws the green rectangle
        window.draw(purpleRect); // 🟪 draws the purple rectangle
        window.draw(brownText);  // Draws "1"
        window.draw(greenText);  // Draws "2"
        window.draw(purpleText); // Draws "3"

        window.display();
    }

    return 0;
}