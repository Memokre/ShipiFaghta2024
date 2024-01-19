
#pragma once
#include <SFML/Graphics.hpp>

#include "MatchState.h"


/*
the phase where players sets up their field
*/



//the ship data determines the choices of ships
class ShipData {
public:
    struct Ship {
        std::string name;
        int length;
        int count;
    };
    std::vector<Ship> ships;
    ShipData() {
        initializeShips();
    }

    void initializeShips() {
        ships = {
            {"Carrier", 4, 1},
            {"Cruiser", 3, 1},
            {"Destroyer", 2, 2},
            {"Frigate", 1, 4}
        };
    }
    Ship getNextShip() {
        for (auto& ship : ships) {
            if (ship.count > 0) {
                ship.count--; // Decrement ship count
                return ship;
            }
        }
        return {};
    }


};


class SetupState {
public:
    //renders grid, colours are based off the value
    void renderGrid(sf::RenderWindow& window, const std::vector<std::vector<char>>& playerField, const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing) {
        for (size_t i = 0; i < playerField.size(); ++i) {
            for (size_t j = 0; j < playerField[i].size(); ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(gridStartPosition.x + i * (cellSize + cellSpacing), gridStartPosition.y + j * (cellSize + cellSpacing));
                if (playerField[i][j] == 'E') {
                    cell.setFillColor(sf::Color::Blue);
                }
                else if (playerField[i][j] == 'S') {
                    cell.setFillColor(sf::Color(128, 128, 128)); // Grey color
                }
                else if (playerField[i][j] == 'H') {
                    cell.setFillColor(sf::Color::Red);
                }
                else if (playerField[i][j] == 'M') {
                    cell.setFillColor(sf::Color(0, 150, 150, 255));
                }
                window.draw(cell);
            }
        }
    }
    // handles the visual and "backend" part of the ship placing for both keyboard and mouse
    void placeShip(sf::RenderWindow& window, std::vector<std::vector<char>>& playerField, const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing, sf::Font& fnt) {
        ShipData shipData; // Initialize ShipData class
        InitializeState ini;
        ini.startall();
        ini.getSetupMusic().setLoop(true);
        ini.getSetupMusic().play();
        // Variables to store ship length and count
        int shipLength = 0;
        int shipCount = 0;

        // Get ship length and count from ShipData class
        ShipData::Ship nextShip = shipData.getNextShip();
        if (!nextShip.name.empty()) {
            shipLength = nextShip.length;
            shipCount = nextShip.count;
        }

        std::vector<std::vector<char>> shipGrid(10, std::vector<char>(10, 'E')); // Initialize ship placement grid

        sf::RectangleShape placementRect(sf::Vector2f(cellSize * shipLength + (cellSpacing * (shipLength - 1)), cellSize));
        placementRect.setFillColor(sf::Color::Transparent);
        placementRect.setOutlineThickness(2.0f);

        sf::Vector2f placementCursorPos = gridStartPosition;
        bool verticalOrientation = true;

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f relativePosition = static_cast<sf::Vector2f>(mousePosition) - gridStartPosition;

                    float adjustedX = std::max(0.0f, std::min(relativePosition.x, static_cast<float>((cellSize + cellSpacing) * 9)));
                    float adjustedY = std::max(0.0f, std::min(relativePosition.y, static_cast<float>((cellSize + cellSpacing) * 9)));

                    int gridX = static_cast<int>(adjustedX / (cellSize + cellSpacing));
                    int gridY = static_cast<int>(adjustedY / (cellSize + cellSpacing));

                    // Update placementCursorPos based on the mouse position
                    placementCursorPos.x = gridStartPosition.x + gridX * (cellSize + cellSpacing);
                    placementCursorPos.y = gridStartPosition.y + gridY * (cellSize + cellSpacing);
                }
                else if (event.type == sf::Event::KeyPressed) {

                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                        // Move placement rectangle upwards if within grid bounds
                        if (placementCursorPos.y > gridStartPosition.y) {
                            placementCursorPos.y -= cellSize + cellSpacing;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        // Move placement rectangle to the left if within grid bounds
                        if (placementCursorPos.x > gridStartPosition.x) {
                            placementCursorPos.x -= cellSize + cellSpacing;
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                        if (verticalOrientation) {
                            // Move placement rectangle downwards if within grid bounds
                            if (placementCursorPos.y < gridStartPosition.y + (9 * (cellSize + cellSpacing)) - (cellSize * shipLength)) {
                                placementCursorPos.y += cellSize + cellSpacing;
                            }
                        }
                        else {
                            // Move placement rectangle downwards if within grid bounds
                            if (placementCursorPos.y < gridStartPosition.y + (9 * (cellSize + cellSpacing))) {
                                placementCursorPos.y += cellSize + cellSpacing;
                            }
                        }
                    }

                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        if (verticalOrientation) {
                            // Move placement rectangle to the right if within grid bounds
                            if (placementCursorPos.x < gridStartPosition.x + (9 * (cellSize + cellSpacing))) {
                                placementCursorPos.x += cellSize + cellSpacing;
                            }
                        }
                        else {
                            // Move placement rectangle to the right if within grid bounds
                            if (placementCursorPos.x < gridStartPosition.x + (9 * (cellSize + cellSpacing)) - (cellSize * shipLength)) {
                                placementCursorPos.x += cellSize + cellSpacing;
                            }
                        }
                    }
                    else if (event.key.code == sf::Keyboard::Space) {
                        // Toggle orientation
                        verticalOrientation = !verticalOrientation;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                        bool canPlaceShip = true;
                        // Check if all coordinates covered by the ship are empty ('E')
                        for (int i = 0; i < shipLength; ++i) {
                            for (int j = 0; j < shipLength; ++j) {
                                int x = (placementCursorPos.x - gridStartPosition.x) / (cellSize + cellSpacing) + i;
                                int y = (placementCursorPos.y - gridStartPosition.y) / (cellSize + cellSpacing) + j;

                                if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                                    if (playerField[x][y] != 'E') {
                                        canPlaceShip = false;
                                        break; // Break inner loop
                                    }
                                    // Check adjacent positions for existing ships
                                    if ((x > 0 && playerField[x - 1][y] == 'S') || // Check left
                                        (x < 9 && playerField[x + 1][y] == 'S') || // Check right
                                        (y > 0 && playerField[x][y - 1] == 'S') || // Check up
                                        (y < 9 && playerField[x][y + 1] == 'S')) { // Check down
                                        canPlaceShip = false;
                                        break; // Break inner loop
                                    }
                                }
                                else {
                                    canPlaceShip = false;
                                    break; // Break inner loop
                                }
                            }
                            if (!canPlaceShip) {
                                break; // Break outer loop
                            }
                        }

                        // If all coordinates are empty, place the ship
                        if (canPlaceShip) {
                            // Update the playerField grid with the ship's position
                            for (int i = 0; i < shipLength; ++i) {
                                int x = (placementCursorPos.x - gridStartPosition.x) / (cellSize + cellSpacing) + (verticalOrientation ? 0 : i);
                                int y = (placementCursorPos.y - gridStartPosition.y) / (cellSize + cellSpacing) + (verticalOrientation ? i : 0);
                                playerField[x][y] = 'S'; // Mark ship position
                            }

                            // Decrement ship count and fetch the next ship
                            ShipData::Ship nextShip = shipData.getNextShip();
                            if (!nextShip.name.empty()) {
                                shipLength = nextShip.length;
                                shipCount = nextShip.count;
                            }
                            else {
                                ini.getSetupMusic().stop();
                                MatchState matchState;
                                matchState.startMatch(playerField, window, fnt);
                                break;
                            }
                        }

                    }
                }
                                    
                     
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Right) {
                    // Right mouse button logic (toggle orientation)
                    verticalOrientation = !verticalOrientation;
    }
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        bool canPlaceShip = true;
                        // Check if all coordinates covered by the ship are empty ('E')
                        for (int i = 0; i < shipLength; ++i) {
                            for (int j = 0; j < shipLength; ++j) {
                                int x = (placementCursorPos.x - gridStartPosition.x) / (cellSize + cellSpacing) + i;
                                int y = (placementCursorPos.y - gridStartPosition.y) / (cellSize + cellSpacing) + j;

                                if (x >= 0 && x < 10 && y >= 0 && y < 10) {
                                    if (playerField[x][y] != 'E') {
                                        canPlaceShip = false;
                                        break; // Break inner loop
                                    }
                                    // Check adjacent positions for existing ships
                                    if ((x > 0 && playerField[x - 1][y] == 'S') || // Check left
                                        (x < 9 && playerField[x + 1][y] == 'S') || // Check right
                                        (y > 0 && playerField[x][y - 1] == 'S') || // Check up
                                        (y < 9 && playerField[x][y + 1] == 'S')) { // Check down
                                        canPlaceShip = false;
                                        break; // Break inner loop
                                    }
                                }
                                else {
                                    canPlaceShip = false;
                                    break; // Break inner loop
                                }
                            }
                            if (!canPlaceShip) {
                                break; // Break outer loop
                            }
                        }

                        // If all coordinates are empty, place the ship
                        if (canPlaceShip) {
                            // Update the playerField grid with the ship's position
                            for (int i = 0; i < shipLength; ++i) {
                                int x = (placementCursorPos.x - gridStartPosition.x) / (cellSize + cellSpacing) + (verticalOrientation ? 0 : i);
                                int y = (placementCursorPos.y - gridStartPosition.y) / (cellSize + cellSpacing) + (verticalOrientation ? i : 0);
                                playerField[x][y] = 'S'; // Mark ship position
                            }

                            // Decrement ship count and fetch the next ship
                            ShipData::Ship nextShip = shipData.getNextShip();
                            if (!nextShip.name.empty()) {
                                shipLength = nextShip.length;
                                shipCount = nextShip.count;
                            }
                            else {
                                ini.getSetupMusic().stop();
                                MatchState matchState;
                                matchState.startMatch(playerField, window, fnt);
                                break;
                            }
                        }
                    }
                }

            }


            // Render ship placement grid
            window.clear();
            renderGrid(window, playerField, gridStartPosition, cellSize, cellSpacing); // Using playerField
            placementRect.setSize(verticalOrientation ?
                sf::Vector2f(cellSize, cellSize * shipLength + (cellSpacing * (shipLength - 1))) :
                sf::Vector2f(cellSize * shipLength + (cellSpacing * (shipLength - 1)), cellSize));
            placementRect.setPosition(placementCursorPos);
            window.draw(placementRect);
            window.display();

        }
    }

    /*
    the "core function which connects everythihg together, whilst also including the initialisation of player grid
    */

    void setup(sf::RenderWindow& window, sf::Font& fnt) {
        ShipData shipData; // Initialize ShipData class


        int shipLength = 0;
        int shipCount = 0;

        // Get ship length and count from ShipData class
        ShipData::Ship nextShip = shipData.getNextShip();
        if (!nextShip.name.empty()) {
            shipLength = nextShip.length;
            shipCount = nextShip.count;
        }

        std::vector<std::vector<char>> playerField(10, std::vector<char>(10, 'E')); // Initialize 10x10 grid with 'E'
        const sf::Vector2f gridStartPosition(100.0f, 200.0f);
        const float cellSize = 40.0f;
        const float cellSpacing = 5.0f;



        // Render the grid and handle ship placement
        placeShip(window, playerField, gridStartPosition, cellSize, cellSpacing, fnt);

        // Render the grid after ship placement
        window.clear();
        renderGrid(window, playerField, gridStartPosition, cellSize, cellSpacing);
        window.display(); // Display everything rendered
    }
};
