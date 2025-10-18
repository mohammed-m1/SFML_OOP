#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <set>

// Custom comparator for sf::Vector2i
struct Vector2iComparator {
    bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
        if (lhs.x != rhs.x) return lhs.x < rhs.x;
        return lhs.y < rhs.y;
    }
};

// Function declarations
// sf::CircleShape circleMoveCyan(const sf::Vector2f offset, const float screenWidth, const float screenHeight);
sf::VertexArray makeDisplay(float screenWidth, float screenHeight, const int gridWidth, const float gridSize, const int gridHeight, const sf::Vector2f offset, sf::VertexArray grid);

class Map {
public:
    std::vector<sf::Vector2i> pathCells; // Sequence of {col, row} for the path
    std::set<sf::Vector2i, Vector2iComparator> allowedPlacements; // Allowed {col, row} for building placement with custom comparator
    sf::Color pathColor = sf::Color(100, 100, 100); // Light grey
    sf::Color allowedColor = sf::Color(200, 162, 200); // Light purple
};

Map map1() {
    Map m;
    // Path for map1 (0-based, col 0-15, row 0-11)
    // Entrance: col 0, row 3 (4th from top, if top is row 0)
    // 6 forward: cols 0-5, row 3
    for (int col = 0; col < 6; ++col) m.pathCells.push_back({col, 3});
    // 5 down: rows 4-8, col 5
    for (int row = 4; row < 9; ++row) m.pathCells.push_back({5, row});
    // 5 ahead: cols 6-10, row 8
    for (int col = 6; col < 11; ++col) m.pathCells.push_back({col, 8});
    // 5 up: rows 7-3, col 10
    for (int row = 7; row > 2; --row) m.pathCells.push_back({10, row});
    // 5 forward: cols 11-15, row 3
    for (int col = 11; col < 16; ++col) m.pathCells.push_back({col, 3});

    // Allowed placements from user
    m.allowedPlacements = {
        {3,4},{3,5},{4,4},{4,5},
        {7,6},{7,7},{8,6},{8,7},
        {11,4},{11,5},{12,4},{12,5}
    };

    return m;
}

Map map2() {
    Map m;
    // Path: col 0-15 row 5
    for (int col = 0; col < 16; ++col) m.pathCells.push_back({col, 5});

    // Allowed placements from user
    m.allowedPlacements = {
        {3,4}, {4,4}, {5,4}, {9,4}, {10,4}, {11,4}, {6,6}, {7,6}, {8,6}
    };

    return m;
}

Map map3() {
    Map m;
    // Path: col 0-6 row 9, row 9-2 col 6, col 6-15 row 2
    for (int col = 0; col < 7; ++col) m.pathCells.push_back({col, 9});
    for (int row = 8; row > 1; --row) m.pathCells.push_back({6, row});
    for (int col = 7; col < 16; ++col) m.pathCells.push_back({col, 2});

    // Allowed placements from user
    m.allowedPlacements = {
        {5,5},{5,6},{5,7},{5,8}, {7,3},{7,4},{8,3},{8,4}
    };

    return m;
}

void drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
    sf::Text title(font, "Tower Defence Game", 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f((1000.0f - title.getLocalBounds().size.x) / 2.0f, 300.0f));

    sf::Text prompt(font, "Press key 1, 2, or 3 for map selection", 30);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(sf::Vector2f((1000.0f - prompt.getLocalBounds().size.x) / 2.0f, 400.0f));

    window.draw(title);
    window.draw(prompt);
}

// sf::CircleShape circleMoveCyan(const sf::Vector2f offset, const float screenWidth, const float screenHeight) {
//     sf::CircleShape circle(20.0f);
//     circle.setFillColor(sf::Color::Cyan);
//     circle.setOrigin(sf::Vector2f{circle.getRadius(), circle.getRadius()});
//     circle.setPosition(sf::Vector2f{offset.x + screenWidth - 50.0f, offset.y + screenHeight / 2.0f}); // Start on right
//     return circle;
// }

sf::VertexArray makeDisplay(float screenWidth, float screenHeight, const int gridWidth, const float gridSize, const int gridHeight, const sf::Vector2f offset, sf::VertexArray grid) {
    for (int x = 0; x <= gridWidth; ++x) {
        float xpos = static_cast<float>(x) * gridSize + offset.x;
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{xpos, offset.y + screenHeight}, sf::Color::Green });
    }

    for (int y = 0; y <= gridHeight; ++y) {
        float ypos = static_cast<float>(y) * gridSize + offset.y;
        grid.append(sf::Vertex{ sf::Vector2f{offset.x, ypos}, sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f{offset.x + screenWidth, ypos}, sf::Color::Green });
    }
    return grid;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u{1000, 1000}), "SFML Grid Centered");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        return 1;
    }

    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    const sf::Vector2f offset{
        (1000.0f - screenWidth) / 2.0f,
        (1000.0f - screenHeight) / 2.0f
    };

    const float gridSize = 50.0f;
    const int numCols = static_cast<int>(screenWidth / gridSize);
    const int numRows = static_cast<int>(screenHeight / gridSize);

    sf::VertexArray gridLines(sf::PrimitiveType::Lines);
    gridLines = makeDisplay(screenWidth, screenHeight, numCols, gridSize, numRows, offset, gridLines);

    bool inMainMenu = true;
    Map currentMap;

    std::vector<std::vector<sf::Color>> gridColors(numRows, std::vector<sf::Color>(numCols, sf::Color::Black));

    // sf::CircleShape circle = circleMoveCyan(offset, screenWidth, screenHeight);

    sf::RectangleShape brownRect;
    brownRect.setSize(sf::Vector2f{60.0f, 60.0f});
    brownRect.setFillColor(sf::Color(139, 69, 19));
    brownRect.setPosition(sf::Vector2f{offset.x + 60.0f, offset.y + screenHeight + 60.0f});
    brownRect.setOutlineColor(sf::Color::Transparent);
    brownRect.setOutlineThickness(2.0f);

    sf::RectangleShape greenRect;
    greenRect.setSize(sf::Vector2f{60.0f, 60.0f});
    greenRect.setFillColor(sf::Color(0, 100, 0));
    greenRect.setPosition(sf::Vector2f{offset.x + 130.0f, offset.y + screenHeight + 60.0f});
    greenRect.setOutlineColor(sf::Color::Transparent);
    greenRect.setOutlineThickness(2.0f);

    sf::RectangleShape purpleRect;
    purpleRect.setSize(sf::Vector2f{60.0f, 60.0f});
    purpleRect.setFillColor(sf::Color(128, 0, 128));
    purpleRect.setPosition(sf::Vector2f{offset.x + 200.0f, offset.y + screenHeight + 60.0f});
    purpleRect.setOutlineColor(sf::Color::Transparent);
    purpleRect.setOutlineThickness(2.0f);

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

    std::vector<sf::CircleShape> placedCircles;
    sf::Color selectedColor = sf::Color(128, 0, 128);
    bool colorSelected = false;

    sf::Clock clock;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            if (!eventOpt) continue; // Skip if no event
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) {
                window.close();
            }

            if (inMainMenu == true) {
                if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
                    if (keyPress->scancode == sf::Keyboard::Scan::Num1) {
                        currentMap = map1();
                        inMainMenu = false;
                    } else if (keyPress->scancode == sf::Keyboard::Scan::Num2){
                        currentMap = map2();
                        inMainMenu = false;
                    } else if (keyPress->scancode == sf::Keyboard::Scan::Num3) {
                        currentMap = map3();
                        inMainMenu = false;
                    }
                }
            } else {
                if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseButton->button == sf::Mouse::Button::Left) {
                        sf::Vector2f pos(static_cast<float>(mouseButton->position.x), static_cast<float>(mouseButton->position.y));

                        brownRect.setOutlineColor(sf::Color::Transparent);
                        greenRect.setOutlineColor(sf::Color::Transparent);
                        purpleRect.setOutlineColor(sf::Color::Transparent);

                        if (brownRect.getGlobalBounds().contains(pos)) {
                            selectedColor = sf::Color(139, 69, 19); 
                            colorSelected = true;
                            brownRect.setOutlineColor(sf::Color::White);
                        } else if (greenRect.getGlobalBounds().contains(pos)) {
                            selectedColor = sf::Color(0, 100, 0);
                            colorSelected = true;
                            greenRect.setOutlineColor(sf::Color::White);
                        } else if (purpleRect.getGlobalBounds().contains(pos)) {
                            selectedColor = sf::Color(128, 0, 128);
                            colorSelected = true;
                            purpleRect.setOutlineColor(sf::Color::White);
                        } else if (colorSelected == true && pos.x >= offset.x && pos.x <= offset.x + screenWidth &&
                                   pos.y >= offset.y && pos.y <= offset.y + screenHeight) {
                            int cellX = static_cast<int>((pos.x - offset.x) / gridSize);
                            int cellY = static_cast<int>((pos.y - offset.y) / gridSize);
                            sf::Vector2i cell(cellX, cellY);
                            if (placedCircles.size() < 3 && currentMap.allowedPlacements.count(cell)) {
                                float centerX = offset.x + static_cast<float>(cellX) * gridSize + gridSize / 2.0f;
                                float centerY = offset.y + static_cast<float>(cellY) * gridSize + gridSize / 2.0f;
                                sf::CircleShape newCircle(10.0f);
                                newCircle.setFillColor(selectedColor);
                                newCircle.setOrigin(sf::Vector2f{10.0f, 10.0f});
                                newCircle.setPosition(sf::Vector2f{centerX, centerY});
                                placedCircles.push_back(newCircle);
                                colorSelected = false;
                                brownRect.setOutlineColor(sf::Color::Transparent);
                                greenRect.setOutlineColor(sf::Color::Transparent);
                                purpleRect.setOutlineColor(sf::Color::Transparent);
                            }
                        }
                    }
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();
        float circleSpeed = -200.0f;
        circle.move(sf::Vector2f{circleSpeed * deltaTime, 0.0f});

        if (circle.getPosition().x < offset.x - circle.getRadius() * 2.0f) {
            circle.setPosition(sf::Vector2f{offset.x + screenWidth + circle.getRadius() * 2.0f, offset.y + screenHeight / 2.0f});
        }

        window.clear(sf::Color::Black);
        if (inMainMenu) {
            drawMainMenu(window, font);
        } else {
            for (int row = 0; row < numRows; ++row) {
                for (int col = 0; col < numCols; ++col) {
                    sf::RectangleShape cellRect(sf::Vector2f{gridSize, gridSize});
                    cellRect.setPosition(sf::Vector2f(offset.x + col * gridSize, offset.y + row * gridSize));
                    cellRect.setFillColor(currentMap.pathColor);
                    if (currentMap.allowedPlacements.count({col, row})) {
                        cellRect.setFillColor(currentMap.allowedColor);
                    }
                    window.draw(cellRect);
                }
            }

            window.draw(gridLines);
            window.draw(circle);
            for (const auto& p : placedCircles) {
                window.draw(p);
            }
            window.draw(brownRect);
            window.draw(greenRect);
            window.draw(purpleRect);
            window.draw(brownText);
            window.draw(greenText);
            window.draw(purpleText);
        }

        window.display();
    }

    return 0;
}