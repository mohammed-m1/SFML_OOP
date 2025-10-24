#include <SFML/Graphics.hpp>
#include <optional> //event handling 
#include <vector> //dsa
#include <set> //dsa
#include <algorithm> //for std::ifnd
#include <fstream>
#include <cmath>
#include <iostream>
#include "ballista.h"
#include "farm.h"
#include "building.h"
#include "enemy.h"
#include "mage_tower.h"
#include "goblin.h"
#include "ogre.h"
#include "skeleton.h"
#include "player.h"
#include "wave.h"
#include "player.h"



class Projectile { //quick getting all the info
    public:
        sf::CircleShape shape;
        sf::Vector2f velocity;
        float damage;
        std::string damType;
};

//projectile calcs distance between 2 points. find the nearest target use
float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
    sf::Vector2f d = a - b;
    return sqrt(d.x * d.x + d.y * d.y); //pytahgroas
}
// unit legnth vecrotr for computing.    finds direction to make speed constant otehrwise skeweed.
sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = sqrt(v.x * v.x + v.y * v.y);
    if (len == 0) return {0, 0};
    return v / len;
}

void saveGame(const std::vector<building*>& buildings, const player& player1, int shownWaveNo) {
        try { //try catch 
        std::ofstream file("save.txt");
        if (!file) throw std::runtime_error("Failed to open save file");

        file << player1.get_money() << " "
             << player1.get_health() << " "
             << shownWaveNo << "\n";

        std::cout << "Game saved successfully.\n";
    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << "\n";
    }
}

// updates the towers and buildings role like money generation or the damage projectiles thing
void updateBuildingsAndProjectiles(
    std::vector<building*>& buildings,
    std::vector<enemy>& enemies,
    std::vector<Projectile>& projectiles,
    float dt,  //frametime timeline
    float projectileSpeed,
    player& player1
) {
    for (auto* b : buildings) { // make it a pointer and real data type like farm or mage basis sam e as for (building* b : buildings) { }
        //auto lets the compiler deduce the exact type automatically; used for cleaner code and flexibility

    b->update_shot_timer(dt); //update shot timing for individual cool down of of tower
    if (dynamic_cast<farm*>(b)) {// dynamic cast run time safety cuz of errors faced so likw when farm is the b farm is run cuz it acts diff
        player1.add_money(static_cast<farm*>(b)->get_farmRate() * dt / 5.f); // need specifc time only.
        continue; // skip projectile firing logic
    }

    if (!b->ready_to_fire()){
        continue;
    } ;
        // Find nearest enemy   
        enemy* target = nullptr;
        float bestDist = 1e9; //BIG NUMBER LOGIC    
        for (auto& e : enemies) {
            if (!e.isAlive()){ 
                continue;    ////////HUMANZIE THIS SHIT WTG
            }
            float d = distance(b->get_position(), e.get_position());
            if (d <= b->get_range() * 50 && d < bestDist) { // range in tiles
                target = &e;
                bestDist = d;
            }
        }

        if (target) {// target is found and so make a projecteclile to hit it and shooting logic ish
            Projectile p;
            p.shape = sf::CircleShape(5.f);
            p.shape.setOrigin({5.f, 5.f});
            p.shape.setPosition(b->get_position());
            sf::Vector2f dir = normalize(target->get_position() - b->get_position()); //find direction towards it and then shoot
            p.velocity = dir * projectileSpeed;
            p.damage = b->get_damage();
            p.damType = b->get_damageType();
            projectiles.push_back(p);
            b->reset_shot_timer(); //cooldown
        }

        // Special case: farms generate money
        // if (dynamic_cast<farm*>(b)) {
        //     player1.add_money(static_cast<farm*>(b)->get_farmRate() * dt / 5.f);
        // }
    }
}


// Comparator for sf::
class Vector2iComparator {
    public:
        bool operator()(const sf::Vector2i& lhs, const sf::Vector2i& rhs) const {
            if (lhs.x != rhs.x) return lhs.x < rhs.x;
            return lhs.y < rhs.y; // return true useed to see if x and y are small of this vector
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
    for (int col = 0; col < 6; col++) m.pathCells.push_back({col, 3}); //appends the coordinates/blocks for making the map in loop
    for (int row = 3; row < 9; row++) m.pathCells.push_back({5, row});
    for (int col = 5; col < 11; col++) m.pathCells.push_back({col, 8});
    for (int row = 8; row > 2; row--) m.pathCells.push_back({10, row});
    for (int col = 11; col < 16; col++) m.pathCells.push_back({col, 3});
    m.allowedPlacements = {{3,4},{3,5},{4,4},{4,5},{7,6},{7,7},{8,6},{8,7},{11,4},{11,5},{12,4},{12,5}}; //places wehre you can place.
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
    for (int col = 0; col < 7; col++) m.pathCells.push_back({col, 9});
    for (int row = 9; row > 1; row--) m.pathCells.push_back({6, row});
    for (int col = 6; col < 16; col++) m.pathCells.push_back({col, 2});
    m.allowedPlacements = {{5,5},{5,6},{5,7},{5,8},{7,3},{7,4},{8,3},{8,4}};
    return m;
}

//  call by ref so i can modify adn use it, gmaes name and 1,2,3 guide draw the tile and menu text
void drawMainMenu(sf::RenderWindow& window, sf::Font& font) {
    sf::Text title(font, "Tower Defence Game", 40);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f((1000.f ) / 2.f, 300.f));
    title.setPosition(sf::Vector2f((1000.f - ( title.getLocalBounds().size.x - title.getLocalBounds().position.x)) / 2.f, 300.f)); //centre the text



    sf::Text prompt(font, "Press 1, 2, or 3 to choose a map", 30);
    prompt.setFillColor(sf::Color::White);
    prompt.setPosition(sf::Vector2f((1000 - prompt.getLocalBounds().size.x) / 2.0f, 400.0f));
    

    window.draw(title);
    window.draw(prompt);
}

// makes the grid lines by using the vertices and returns the array with all the lines fr grid
sf::VertexArray makeDisplay(float screenWidth, float screenHeight, int gridWidth, float gridSize, int gridHeight, const sf::Vector2f& offset) {
    sf::VertexArray grid(sf::PrimitiveType::Lines);
    for (int x = 0; x <= gridWidth; x++) {
        float xpos = x * gridSize + offset.x;
        grid.append(sf::Vertex{ sf::Vector2f(xpos, offset.y), sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f(xpos, offset.y + screenHeight), sf::Color::Green });
    }
    for (int y = 0; y <= gridHeight; y++) {
        float ypos = y * gridSize + offset.y;
        grid.append(sf::Vertex{ sf::Vector2f(offset.x, ypos), sf::Color::Green });
        grid.append(sf::Vertex{ sf::Vector2f(offset.x + screenWidth,  ypos), sf::Color::Green });
    }
    return grid;
}

// enum type for only having 2 options mutualy exclusiive.
enum class GameState { MainMenu, Playing };

// to get the game from main menu to the map using keys pressed.
void handleMenuEvents(
    const sf::Event& event, 
    GameState& state, 
    Map& currentMap,
    WaveManager*& currentWaveManager, // pass pointer by reference
    const std::vector<Wave>& map1Waves,
    const std::vector<Wave>& map2Waves,
    const std::vector<Wave>& map3Waves
) {
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->scancode == sf::Keyboard::Scan::Num1) { 
            currentMap = map1(); 
            currentWaveManager = new WaveManager(map1Waves, 50.f); // 50.f = enemySpeed
            state = GameState::Playing; 
        }
        else if (keyPress->scancode == sf::Keyboard::Scan::Num2) { 
            currentMap = map2(); 
            currentWaveManager = new WaveManager(map2Waves, 50.f);
            state = GameState::Playing; 
        }
        else if (keyPress->scancode == sf::Keyboard::Scan::Num3) { 
            currentMap = map3(); 
            currentWaveManager = new WaveManager(map3Waves, 50.f);
            state = GameState::Playing; 
        }
    }
}

// m menu go to menu section
void handleGameEvents(
    const sf::Event& event, GameState& state,
    std::vector<sf::CircleShape>& placedCircles, sf::Color& selectedColor, bool& colorSelected,
    const Map& currentMap, const sf::Vector2f& offset, float screenWidth, float screenHeight, float gridSize,
    sf::RectangleShape& brownRect, sf::RectangleShape& greenRect, sf::RectangleShape& purpleRect,
    int numCirclesAllowed,
    std::vector<building*>& buildings, std::vector<enemy>& enemies,// actual placed buildings
    player& player1, WaveManager*& currentWaveManager, bool& waveActive, int& shownWaveNo,float& waveTimer
) {
    // main menu code
    if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPress->scancode == sf::Keyboard::Scan::S) {
           saveGame(buildings, player1, shownWaveNo);
        }
        if (keyPress->scancode == sf::Keyboard::Scan::M) {
            state = GameState::MainMenu;
            placedCircles.clear();
            buildings.clear(); // remove all buildings
            enemies.clear();
            if (currentWaveManager) {
                delete currentWaveManager;
                currentWaveManager = nullptr;
            }
            player1 = player();
            player1.set_money(500);
            player1.set_health(15);
            waveActive = false;
            shownWaveNo = 0;
            waveTimer = 0.f;
            // projectiles.clear();
            // placedCircles.clear();
            colorSelected = false;
            brownRect.setOutlineColor(sf::Color::Transparent);
            greenRect.setOutlineColor(sf::Color::Transparent);
            purpleRect.setOutlineColor(sf::Color::Transparent);
            return;
        }
    }

    // placing buildings click mouse 
    if (const auto* mouseButton = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButton->button == sf::Mouse::Button::Left) {
            sf::Vector2f pos(static_cast<float>(mouseButton->position.x),
                             static_cast<float>(mouseButton->position.y));

            // graphic border thing
            brownRect.setOutlineColor(sf::Color::Transparent);
            greenRect.setOutlineColor(sf::Color::Transparent);
            purpleRect.setOutlineColor(sf::Color::Transparent);

            // select building 
            if (brownRect.getGlobalBounds().contains(pos)) {
                selectedColor = sf::Color(139, 69, 19); // Brown  Ballista
                colorSelected = true;
                brownRect.setOutlineColor(sf::Color::White);
            } 
            else if (greenRect.getGlobalBounds().contains(pos)) {
                selectedColor = sf::Color(0, 100, 0); // Green  Farm
                colorSelected = true;
                greenRect.setOutlineColor(sf::Color::White);
            } 
            else if (purpleRect.getGlobalBounds().contains(pos)) {
                selectedColor = sf::Color(128, 0, 128); // Purple  Mage Tower
                colorSelected = true;
                purpleRect.setOutlineColor(sf::Color::White);
            } 

            // place building in the centre of the box
            else if (colorSelected &&
                     pos.x >= offset.x && pos.x <= offset.x + screenWidth &&
                     pos.y >= offset.y && pos.y <= offset.y + screenHeight)
            {
                int cellX = static_cast<int>((pos.x - offset.x) / gridSize);
                int cellY = static_cast<int>((pos.y - offset.y) / gridSize);
                sf::Vector2i cell(cellX, cellY);

                //upgrading the building code
                building* existingBuilding = nullptr;  
                for (auto* b : buildings) {  
                    sf::Vector2f posB = b->get_position();  
                    int existingX = static_cast<int>((posB.x - offset.x) / gridSize);  
                    int existingY = static_cast<int>((posB.y - offset.y) / gridSize);  
                    if (existingX == cellX && existingY == cellY) {  
                        if (b->get_color() == selectedColor) {  
                            existingBuilding = b;  
                        }
                        break;  
                    }
                }

                if (existingBuilding) {  
                    if (player1.get_money() >= existingBuilding->get_cost()) {
                            player1.add_money(-existingBuilding->get_cost()); // deduct cost
                            existingBuilding->upgrade(); 
                            std::cout << "Upgraded existing building at (" << cellX << "," << cellY << ")\n"; 
                        } else {
                            std::cout << "Nah no money";
                        }
                     
                    return; // Skip new placement 
                }



                // Check placement possible if there is correct box and total building is less. size allows to not keep a count of it
                if (buildings.size() < numCirclesAllowed && currentMap.allowedPlacements.count(cell)) {
                    float centerX = offset.x + cellX * gridSize + gridSize / 2.0f;
                    float centerY = offset.y + cellY * gridSize + gridSize / 2.0f;

                    // create it bldg
                    building* newBuilding = nullptr;

                    if (selectedColor == sf::Color(139, 69, 19)) {
                        newBuilding = new ballista(buildings.size()); // Ballista
                    }
                    

                    else if (selectedColor == sf::Color(0, 100, 0)) {
                        newBuilding = new mage_tower(buildings.size()); // farm
                    }
                    else if (selectedColor == sf::Color(128, 0, 128)) {
                        newBuilding = new farm(buildings.size()); // mage
                    }
                    if (newBuilding) {
                        // Check if player has enough money before placing
                        if (player1.get_money() >= newBuilding->get_cost()) {
                            player1.add_money(-newBuilding->get_cost()); // deduct cost
                            newBuilding->set_position(sf::Vector2f(centerX, centerY));
                            buildings.push_back(newBuilding);
                        } else {
                            delete newBuilding; // not enough money
                            return; // cancel it
                        }
                    }

                    // circle visual of bldg
                    sf::CircleShape newCircle(10.0f);
                    newCircle.setFillColor(selectedColor);
                    newCircle.setOrigin({10.0f, 10.0f});
                    newCircle.setPosition({centerX, centerY});
                    placedCircles.push_back(newCircle);

                    // Reset color  
                    colorSelected = false;
                    brownRect.setOutlineColor(sf::Color::Transparent);
                    greenRect.setOutlineColor(sf::Color::Transparent);
                    purpleRect.setOutlineColor(sf::Color::Transparent);
                }
            }
        }
    }
}

// takes everything and draw it out
void drawGame(
    sf::RenderWindow& window, const Map& map, float gridSize, int numRows, int numCols,
    const sf::Vector2f& offset, float screenWidth, float screenHeight, const sf::VertexArray& gridLines,
    const std::vector<sf::CircleShape>& placedCircles,
    const sf::RectangleShape& brownRect, const sf::RectangleShape& greenRect, const sf::RectangleShape& purpleRect,
    const sf::Text& brownText, const sf::Text& greenText, const sf::Text& purpleText
) {
    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
            sf::RectangleShape cellRect({gridSize, gridSize});
            cellRect.setPosition(sf::Vector2f(offset.x + col * gridSize, offset.y + row * gridSize));
            sf::Vector2i cell(col, row);
            if (std::find(map.pathCells.begin(), map.pathCells.end(), cell) != map.pathCells.end())
                cellRect.setFillColor(sf::Color(255, 128, 128)); // normal background
            else if (map.allowedPlacements.count(cell))
                cellRect.setFillColor(map.allowedColor); // for buildings placement color
            else
                cellRect.setFillColor(map.pathColor); // if its a path its red in color
            window.draw(cellRect);
        }
    }

    window.draw(gridLines);
    for (const auto& p : placedCircles) window.draw(p);
    window.draw(brownRect);
    window.draw(greenRect); // all the rectangels 
    window.draw(purpleRect);
    window.draw(brownText);
    window.draw(greenText); // the text on them
    window.draw(purpleText);
}


// 
int main() {

    player player1(1);

    sf::RenderWindow window(sf::VideoMode({1000, 1000}), "Tower Defence Game");
    window.setFramerateLimit(60);
    sf::Font font;
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) return 1;  //font check

    const float screenWidth = 800, screenHeight = 600, gridSize = 50; //constant size of the screen in the window to paly
    const sf::Vector2f offset((1000 - screenWidth) / 2, (1000 - screenHeight) / 2);  // offset gaps x and y for the screen inside for calcs
    int numCols = static_cast<int>(screenWidth / gridSize), numRows = static_cast<int>(screenHeight / gridSize); //count of row and col
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

    // Define waves for each map
    std::vector<Wave> map3Waves = { {6, 0, 0}, {3, 3, 1}, {2, 2, 2} };
    std::vector<Wave> map1Waves = { {2, 4, 0}, {3, 2, 0}, {2, 3, 1} };
    std::vector<Wave> map2Waves = { {4, 1, 1}, {2, 4, 1}, {3, 3, 2} };
    WaveManager* currentWaveManager = nullptr;
    bool waveActive = false;
    int shownWaveNo = 0;

    float waveTimer = 0.f;  

    // Game state and playing
    GameState gameState = GameState::MainMenu;
    Map currentMap;
    std::vector<sf::CircleShape> placedCircles;
    sf::Color selectedColor;
    bool colorSelected = false;
    int numCirclesAllowed = 6; // number of buildings that can be placed max



    std::vector<building*> buildings;  ////// array of builidng point datatype  polymorphism
    std::vector<enemy> enemies;
    std::vector<Projectile> projectiles;

    sf::Clock globalClock;

    sf::Text waveText(font, "Wave 1", 30);
    waveText.setPosition(sf::Vector2f(offset.x + 10, offset.y - 40));
    waveText.setFillColor(sf::Color::Magenta);

    sf::Text moneyText( font, "Money : 0" ,18);
    moneyText.setPosition(sf::Vector2f(offset.x + 10, offset.y + screenHeight + 10));
    moneyText.setFillColor(sf::Color::Yellow);

    sf::Text healthText(font, "Health: 0", 18);
    healthText.setPosition(sf::Vector2f(offset.x + 200, offset.y + screenHeight + 10));

    healthText.setFillColor(sf::Color::Red);

    const float enemyBasePixelsPerSecond = 70.f; // mid speed
    const float projectileSpeed = 350.f;

    float farmTimer = 0.f;
    const float farmInterval = 5.f; // 4 seconds per payout


    // main game loop
    while (window.isOpen()) {
    float dt = globalClock.restart().asSeconds(); // Delta time is time passed since last frame....  perframe time
    farmTimer += dt;
    if (farmTimer >= farmInterval) {
        for (auto* b : buildings) { ///auto =  fast way for compiler to checlk the type
            if (auto* f = dynamic_cast<farm*>(b)) { // virtual in base safe runtime check
                player1.add_money(f->get_farmRate());
            }
        }
        farmTimer = 0.f; // reset timer
    }

    while (const std::optional<sf::Event> eventOpt = window.pollEvent()) { //gives the event handling in the window
        if (!eventOpt) continue;
        const sf::Event& event = *eventOpt;

        if (event.is<sf::Event::Closed>()){
             window.close(); // close window
        }
        else if (gameState == GameState::MainMenu) {
            handleMenuEvents(event, gameState, currentMap, currentWaveManager, map1Waves, map2Waves, map3Waves);
        }
        else if (gameState == GameState::Playing) {
            handleGameEvents(event, gameState, placedCircles, selectedColor, colorSelected,
                             currentMap, offset, screenWidth, screenHeight, gridSize,
                             brownRect, greenRect, purpleRect, numCirclesAllowed, buildings,enemies,player1,currentWaveManager,waveActive, shownWaveNo, waveTimer);
            }
    }

    if (gameState == GameState::Playing && currentWaveManager) {
    //Spawn next wave only if no active wave and more remain 
    if (!waveActive && currentWaveManager->hasNextWave()) {
        currentWaveManager->spawnNextWave(enemies, currentMap.pathCells, offset);
        waveActive = true;
        shownWaveNo++;
        waveText.setString("Wave " + std::to_string(shownWaveNo));
    }
    else if (!waveActive && !currentWaveManager->hasNextWave()) {
    // All waves cleared — game over
    sf::Text gameOverText(font, "All Waves Cleared! Returning to Main Menu...", 28);
    gameOverText.setFillColor(sf::Color::Yellow);
    gameOverText.setPosition({offset.x + 100, offset.y + 200});
    
    window.clear(sf::Color::Black);
    window.draw(gameOverText);
    window.display();

    sf::sleep(sf::seconds(3)); 

    // clear all
    enemies.clear();
    projectiles.clear();
    buildings.clear();
    placedCircles.clear();

    waveActive = false;
    shownWaveNo = 0;
    waveTimer = 0.f;

    if (currentWaveManager) {
        delete currentWaveManager;   // free
        currentWaveManager = nullptr;
    }

    player1 = player();  // reset player (money + health)
    moneyText.setString("Money: " + std::to_string(player1.get_money()));
    healthText.setString("Health: " + std::to_string(player1.get_health()));

    gameState = GameState::MainMenu;
    player1.set_money(500);
    player1.set_health(15);
    continue; // restart loop to go back to menu
}

    // Update all enemies along their paths 
   for (auto& e : enemies) { // make data type easier 
    bool reachedEnd = e.update(dt, gridSize, offset);
    if (reachedEnd) {
        player1.add_health(-(e.get_damage())); // player takes damage
        // e.take_damage(9999, "none");          // kill the enemy to remove it
    }
}

    // Check if current wave has been cleared 
    if (waveActive) {
        bool anyAlive = false;
        for (auto& e : enemies) {
            if (e.isAlive()) {
                anyAlive = true;
                break;
            }
        }

        if (!anyAlive) {
            enemies.clear();       // cleanup dead enemies
            waveActive = false;    // start wave
        }
    }

    // Update towers and their projectiles
    updateBuildingsAndProjectiles(buildings, enemies, projectiles, dt, projectileSpeed, player1);
    // Update UI 
    moneyText.setString("Money: " + std::to_string(player1.get_money()));
    healthText.setString("Health: " + std::to_string(player1.get_health()));

    if (player1.get_health() <= 0) {
    sf::Text gameOverText(font, "You Lost! Returning to Main Menu...", 28);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition({offset.x + 100, offset.y + 200});

    window.clear(sf::Color::Black);
    window.draw(gameOverText);
    window.display();
    sf::sleep(sf::seconds(3));
        ///reset if lost 
    enemies.clear();
    projectiles.clear();
    buildings.clear();
    placedCircles.clear();

    waveActive = false;
    shownWaveNo = 0;
    waveTimer = 0.f;

    if (currentWaveManager) {
        delete currentWaveManager;
        currentWaveManager = nullptr;
    }

    player1 = player();
    moneyText.setString("Money: " + std::to_string(player1.get_money()));
    healthText.setString("Health: " + std::to_string(player1.get_health()));
    player1.set_money(500);
    player1.set_health(15);

    gameState = GameState::MainMenu;
    continue;
}


    // Move projectiles
    for (auto& p : projectiles) {
        p.shape.move(p.velocity * dt);
    }
    for (auto& p : projectiles) {
    for (auto& e : enemies) {
        if (!e.isAlive()) continue;

        float dist = distance(p.shape.getPosition(), e.get_position());
        float hitRadius = 15.f; // adjust as needed
        if (dist < hitRadius) {
            e.take_damage(static_cast<int>(p.damage), p.damType);  //static cuz 100% sure this is int and muist be int

            // If enemy dies give money
            if (!e.isAlive()) {
                player1.add_money(20); // reward per kill
            }

            //  rmemove projectile (move off-screen for now) kinda is weird way
            p.shape.setPosition({-100.f, -100.f});
            break; // projectile hits only one enemy
        }
    }
}
    projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
        [&](const Projectile& p) {
            sf::Vector2f pos = p.shape.getPosition();
            return (pos.x < 0 || pos.x > 1000 || pos.y < 0 || pos.y > 1000);
        }), projectiles.end()); // same logic if outside the screen then remove it 
}

// --- Drawing section ---
window.clear(sf::Color::Black);

if (gameState == GameState::MainMenu)
    drawMainMenu(window, font);
else {
    drawGame(window, currentMap, gridSize, numRows, numCols, offset, screenWidth, screenHeight,
             gridLines, placedCircles, brownRect, greenRect, purpleRect,
             brownText, greenText, purpleText);

    // Draw enemies
    for (auto& e : enemies) {
        if (e.isAlive())
            window.draw(e.get_shape());
    }

    // Draw projectiles
    for (auto& p : projectiles) {
        window.draw(p.shape);
    }
    window.draw(moneyText);
    window.draw(healthText);
    window.draw(moneyText);
    window.draw(waveText);
}

window.display();

}
    return 0;
}
