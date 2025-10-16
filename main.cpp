#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    // Create the window with explicit VideoMode
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{800, 600}), "SFML Grid and Circle Moving trial");

    // Define grid properties
    const float gridSize = 50.0f;
    const int gridWidth = static_cast<int>(800.0f / gridSize);
    const int gridHeight = static_cast<int>(600.0f / gridSize);
    sf::VertexArray grid(sf::PrimitiveType::Lines);

    // Create green grid
    for (int x = 0; x <= gridWidth; x++) {
        grid.append(sf::Vertex{sf::Vector2f(static_cast<float>(x) * gridSize, 0.0f), sf::Color::Green});
        grid.append(sf::Vertex{sf::Vector2f(static_cast<float>(x) * gridSize, 600.0f), sf::Color::Green});
    }
    for (int y = 0; y <= gridHeight; y++) {
        grid.append(sf::Vertex{sf::Vector2f(0.0f, static_cast<float>(y) * gridSize), sf::Color::Green});
        grid.append(sf::Vertex{sf::Vector2f(800.0f, static_cast<float>(y) * gridSize), sf::Color::Green});
    }

    // Create circle
    sf::CircleShape circle(25.0f);
    circle.setFillColor(sf::Color::Cyan);
    circle.setPosition(sf::Vector2f(0.0f, 300.0f)); // Start at right, middle of screen
    float circleSpeed = 20.0f; // Move left at 200 pixels per second

    // Clock for timing
    sf::Clock clock;

    while (window.isOpen())
    {
        // Process events
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Update circle position
        float deltaTime = clock.restart().asSeconds();
        circle.move(sf::Vector2f(circleSpeed * deltaTime, 0.0f));

        // Reset circle to right when it moves off-screen
        if (circle.getPosition().x < -circle.getRadius() * 2.0f)
            circle.setPosition(sf::Vector2f(800.0f, 300.0f));

        // Clear window
        window.clear(sf::Color::Black);

        // Draw grid
        window.draw(grid);

        // Draw circle
        window.draw(circle);

        // Display
        window.display();
    }

    return 0;
}