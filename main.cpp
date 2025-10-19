#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <set>
#include <algorithm>

// Comparator for sf::Vector2i in std::set so i can use it for comparing in allowed placements.
class Vector2iComparator {
    public:
        bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
            if (lhs.x != rhs.x) return lhs.x < rhs.x;
            return lhs.y < rhs.y;
        }
};

class Map {
public:
    std::vector<sf::Vector2i> pathCells;
    std::set<sf::Vector2i, Vector2iComparator> allowedPlacements;
    sf::Color pathColor = sf::Color(100, 100, 100); 
    sf::Color allowedColor = sf::Color(200, 162, 200);
};

// 
Map map1() {
    Map m;
    for (int col = 0; col < 6; ++col) m.pathCells.push_back({col, 3});
    for (int row = 3; row < 9; ++row) m.pathCells.push_back({5, row});
    for (int col = 5; col < 11; ++col) m.pathCells.push_back({col, 8});
    for (int row = 8; row > 2; --row) m.pathCells.push_back({10, row});
    for (int col = 11; col < 16; ++col) m.pathCells.push_back({col, 3});
    m.allowedPlacements = {{3,4},{3,5},{4,4},{4,5},{7,6},{7,7},{8,6},{8,7},{11,4},{11,5},{12,4},{12,5}};
    return m;
}

Map map2() {
    Map m;
    for (int col = 0; col < 16; ++col) m.pathCells.push_back({col, 5});
    m.allowedPlacements = {{3,4},{4,4},{5,4},{9,4},{10,4},{11,4},{6,6},{7,6},{8,6}};
    return m;
}

Map map3() {
    Map m;
    for (int col = 0; col < 7; ++col) m.pathCells.push_back({col, 9});
    for (int row = 9; row > 1; --row) m.pathCells.push_back({6, row});
    for (int col = 6; col < 16; ++col) m.pathCells.push_back({col, 2});
    m.allowedPlacements = {{5,5},{5,6},{5,7},{5,8},{7,3},{7,4},{8,3},{8,4}};
    return m;
}

// --- Draw main menu ---
void drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
    sf::Text title(font, "Tower Defence Game", 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f((1000.f ) / 2.f, 300.f)); // i remvoed the minus sgin thing og is 
    title.setPosition(sf::Vector2f((1000.f - ( title.getLocalBounds().size.x - title.getLocalBounds().position.x)) / 2.f, 300.f));



    sf::Text prompt(font, "Press 1, 2, or 3 to choose a map", 30);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(sf::Vector2f((1000 - prompt.getLocalBounds().size.x) / 2.0f, 400.0f));
    

    window.draw(title);
    window.draw(prompt);
}

// --- Utility to draw grid lines ---
sf::VertexArray makeDisplay(float screenWidth, float screenHeight, int gridWidth, float gridSize, int gridHeight, const sf::Vector2f& offset) {
    sf::VertexArray grid(sf::PrimitiveType::Lines);
    for (int x = 0; x <= gridWidth; ++x) {
        float xpos = x * gridSize + offset.x;
        grid.append(sf::Vertex{ sf::Vector2f(xpos, offset.y), sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f(xpos, offset.y + screenHeight), sf::Color::Green });
    }
    for (int y = 0; y <= gridHeight; ++y) {
        float ypos = y * gridSize + offset.y;
        grid.append(sf::Vertex{ sf::Vector2f(offset.x, ypos), sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f(offset.x + screenWidth,  ypos), sf::Color::Green });
    }
    return grid;
}

// --- Game state enum ---
enum class GameState { MainMenu, Playing };

// --- Handle menu events ---
void handleMenuEvents(const sf::Event& event, GameState& state, Map& currentMap) {
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->scancode == sf::Keyboard::Scan::Num1) { currentMap = map1(); state = GameState::Playing; }
        else if (keyPress->scancode == sf::Keyboard::Scan::Num2) { currentMap = map2(); state = GameState::Playing; }
        else if (keyPress->scancode == sf::Keyboard::Scan::Num3) { currentMap = map3(); state = GameState::Playing; }
    }
}

// --- Handle in-game events ---
void handleGameEvents(
    const sf::Event& event, GameState& state,
    std::vector<sf::CircleShape>& placedCircles, sf::Color& selectedColor, bool& colorSelected,
    const Map& currentMap, const sf::Vector2f& offset, float screenWidth, float screenHeight, float gridSize,
    sf::RectangleShape& brownRect, sf::RectangleShape& greenRect, sf::RectangleShape& purpleRect,
    int numCirclesAllowed
) {
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->scancode == sf::Keyboard::Scan::M) {
            state = GameState::MainMenu;
            placedCircles.clear();
            colorSelected = false;
            brownRect.setOutlineColor(sf::Color::Transparent);
            greenRect.setOutlineColor(sf::Color::Transparent);
            purpleRect.setOutlineColor(sf::Color::Transparent);
            return;
        }
    }

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
            } else if (colorSelected && pos.x >= offset.x && pos.x <= offset.x + screenWidth &&
                       pos.y >= offset.y && pos.y <= offset.y + screenHeight) {

                int cellX = static_cast<int>((pos.x - offset.x) / gridSize);
                int cellY = static_cast<int>((pos.y - offset.y) / gridSize);
                sf::Vector2i cell(cellX, cellY);
                if (placedCircles.size() < numCirclesAllowed && currentMap.allowedPlacements.count(cell)) {
                    float centerX = offset.x + cellX * gridSize + gridSize / 2.0f;
                    float centerY = offset.y + cellY * gridSize + gridSize / 2.0f;
                    sf::CircleShape newCircle(10.0f);
                    newCircle.setFillColor(selectedColor);
                    newCircle.setOrigin({10.0f, 10.0f});
                    newCircle.setPosition({centerX, centerY});
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

// --- Draw gameplay environment ---
void drawGame(
    sf::RenderWindow& window, const Map& map, float gridSize, int numRows, int numCols,
    const sf::Vector2f& offset, float screenWidth, float screenHeight, const sf::VertexArray& gridLines,
    const std::vector<sf::CircleShape>& placedCircles,
    const sf::RectangleShape& brownRect, const sf::RectangleShape& greenRect, const sf::RectangleShape& purpleRect,
    const sf::Text& brownText, const sf::Text& greenText, const sf::Text& purpleText
) {
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            sf::RectangleShape cellRect({gridSize, gridSize});
            cellRect.setPosition(sf::Vector2f(offset.x + col * gridSize, offset.y + row * gridSize));
            sf::Vector2i cell(col, row);
            if (std::find(map.pathCells.begin(), map.pathCells.end(), cell) != map.pathCells.end())
                cellRect.setFillColor(sf::Color(255, 128, 128));
            else if (map.allowedPlacements.count(cell))
                cellRect.setFillColor(map.allowedColor);
            else
                cellRect.setFillColor(map.pathColor);
            window.draw(cellRect);
        }
    }

    window.draw(gridLines);
    for (const auto& p : placedCircles) window.draw(p);
    window.draw(brownRect);
    window.draw(greenRect);
    window.draw(purpleRect);
    window.draw(brownText);
    window.draw(greenText);
    window.draw(purpleText);
}

// --- Main function ---
int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "SFML Modular Tower Defence");
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) return 1;

    const float screenWidth = 800, screenHeight = 600, gridSize = 50;
    const sf::Vector2f offset((1000 - screenWidth) / 2, (1000 - screenHeight) / 2);
    int numCols = static_cast<int>(screenWidth / gridSize), numRows = static_cast<int>(screenHeight / gridSize);
    sf::VertexArray gridLines = makeDisplay(screenWidth, screenHeight, numCols, gridSize, numRows, offset);

    // Color selection buttons
    sf::RectangleShape brownRect({60, 60}), greenRect({60, 60}), purpleRect({60, 60});
    brownRect.setFillColor(sf::Color(139, 69, 19));
    greenRect.setFillColor(sf::Color(0, 100, 0));
    purpleRect.setFillColor(sf::Color(128, 0, 128));

    brownRect.setPosition(sf::Vector2f(offset.x + 60, offset.y + screenHeight + 60));
    greenRect.setPosition(sf::Vector2f(offset.x + 130, offset.y + screenHeight + 60));
    purpleRect.setPosition(sf::Vector2f(offset.x + 200, offset.y + screenHeight + 60));

    brownRect.setOutlineColor(sf::Color::Transparent);
    greenRect.setOutlineColor(sf::Color::Transparent);
    purpleRect.setOutlineColor(sf::Color::Transparent);
    brownRect.setOutlineThickness(2);
    greenRect.setOutlineThickness(2);
    purpleRect.setOutlineThickness(2);

    sf::Text brownText(font, "Ballista", 20), greenText(font, "Mage", 20), purpleText(font, "Farm", 20);
    brownText.setFillColor(sf::Color::White);
    greenText.setFillColor(sf::Color::White);
    purpleText.setFillColor(sf::Color::White);

    brownText.setPosition(sf::Vector2f(brownRect.getPosition().x + 5.f, brownRect.getPosition().y + 15.f));
    greenText.setPosition(sf::Vector2f(greenRect.getPosition().x + 10.f, greenRect.getPosition().y + 15.f));
    purpleText.setPosition(sf::Vector2f(purpleRect.getPosition().x + 10.f, purpleRect.getPosition().y + 15.f));


    // Game data
    GameState gameState = GameState::MainMenu;
    Map currentMap;
    std::vector<sf::CircleShape> placedCircles;
    sf::Color selectedColor;
    bool colorSelected = false;
    int numCirclesAllowed = 3;

    // --- Loop ---
    while (window.isOpen()) {
        while (const std::optional<sf::Event> eventOpt = window.pollEvent()) {
            if (!eventOpt) continue;
            const sf::Event& event = *eventOpt;

            if (event.is<sf::Event::Closed>()) window.close();
            else if (gameState == GameState::MainMenu)
                handleMenuEvents(event, gameState, currentMap);
            else if (gameState == GameState::Playing)
                handleGameEvents(event, gameState, placedCircles, selectedColor, colorSelected,
                                 currentMap, offset, screenWidth, screenHeight, gridSize,
                                 brownRect, greenRect, purpleRect, numCirclesAllowed);
        }

        window.clear(sf::Color::Black);
        if (gameState == GameState::MainMenu) drawMainMenu(window, font);
        else drawGame(window, currentMap, gridSize, numRows, numCols, offset, screenWidth, screenHeight,
                      gridLines, placedCircles, brownRect, greenRect, purpleRect,
                      brownText, greenText, purpleText);
        window.display();
    }
    return 0;
}
