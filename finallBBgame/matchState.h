#ifndef MATCHSTATE_H
#define MATCHSTATE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include"matchState.h"
#include "enemy.h"
#include "InitializeState.h"
#include <string>
#include "MenuState.h"
/*
handles everything match related

from the difficulty of AI

through turn themselves 

and the end of the game

*/



class MatchState {
public:
    //shows updated player grid, includes hit and missed tiles
    void renderMatchGrid(sf::RenderWindow& window, const std::vector<std::vector<char>>& playerField, const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing) {
        for (size_t i = 0; i < playerField.size(); ++i) {
            for (size_t j = 0; j < playerField[i].size(); ++j) {
                sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
                cell.setPosition(gridStartPosition.x + i * (cellSize + cellSpacing), gridStartPosition.y + j * (cellSize + cellSpacing));
                if (playerField[i][j] == 'E') {
                    cell.setFillColor(sf::Color::Blue);
                }
                else if (playerField[i][j] == 'S') {
                    cell.setFillColor(sf::Color(128, 128, 128)); // Grey color for ships
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
    //simple screen with three sprites servis as difficulty chosers

    int chooseDifficulty(sf::RenderWindow& window) {

        sf::Texture easyTexture;
        sf::Texture mediumTexture;
        sf::Texture hardTexture;

        int selectedOption = 0;
        InitializeState initState;
        initState.startall();

        // Remove local texture variables
        sf::Sprite easySprite(initState.getEasyTexture());
        sf::Sprite mediumSprite(initState.getMediumTexture());
        sf::Sprite hardSprite(initState.getHardTexture());

        int xoset = 100;
        int yoset = 200;

        easySprite.setPosition(xoset + 50.0f, yoset + 100.0f);
        mediumSprite.setPosition(xoset + 400.0f, yoset + 100.0f);
        hardSprite.setPosition(xoset + 750.0f, yoset + 100.0f);

        sf::RectangleShape indicator(sf::Vector2f(200.0f, 200.0f));
        indicator.setFillColor(sf::Color(0, 0, 0, 0)); //transparent filling of the aiming indicator
        indicator.setOutlineThickness(5.0f); 
        indicator.setOutlineColor(sf::Color::Red); 

        

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return 0;
                }

                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        selectedOption = (selectedOption - 1 + 3) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        selectedOption = (selectedOption + 1) % 3;
                    }
                    else if (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                        
                        return selectedOption;
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) {
                   
                        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);


                        if (easySprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                            selectedOption = 0;
                        }
                        else if (mediumSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                            selectedOption = 1;
                        }
                        else if (hardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                            selectedOption = 2;
                        }

                        return selectedOption;
                    }

                }
                else if (event.type == sf::Event::MouseMoved) {
                    // Get the mouse position
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    // Check if the mouse hovers over the difficulty options and update the selection accordingly
                    if (easySprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                        selectedOption = 0;
                    }
                    else if (mediumSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                        selectedOption = 1;
                    }
                    else if (hardSprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                        selectedOption = 2;
                    }
                }

            }

            window.clear(sf::Color(0, 0, 128)); 
            window.draw(easySprite);
            window.draw(mediumSprite);
            window.draw(hardSprite);
            float indicatorXPosition = xoset + 50.0f + selectedOption * 350.0f;
            float indicatorYPosition = yoset + 100.0f;

            indicator.setPosition(indicatorXPosition, indicatorYPosition);

            window.draw(indicator);

            window.display();
        }

        return selectedOption; 

    }


    void sink(std::vector<std::vector<char>>& grid) {
        int rows = grid.size();
        int cols = grid[0].size();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == 'H') {
                    bool hasAdjacentS = false;

            
                    for (int k = std::max(0, j - 1); k <= std::min(cols - 1, j + 1); ++k) {
                        if (grid[i][k] == 'S') {
                            hasAdjacentS = true;
                            break;
                        }
                    }
                    for (int k = std::max(0, i - 1); k <= std::min(rows - 1, i + 1); ++k) {
                        if (grid[k][j] == 'S') {
                            hasAdjacentS = true;
                            break;
                        }
                    }

                    if (!hasAdjacentS) {
       
                        for (int k = std::max(0, i - 1); k <= std::min(rows - 1, i + 1); ++k) {
                            if (k != i) { 
                                if (grid[k][j] == 'E') {
                                    grid[k][j] = 'M';
                                }
                            }
                        }
                        for (int l = std::max(0, j - 1); l <= std::min(cols - 1, j + 1); ++l) {
                            if (l != j) { 
                                if (grid[i][l] == 'E') {
                                    grid[i][l] = 'M';
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    void shooty(sf::RenderWindow& window, std::vector<std::vector<char>>& playerField, std::vector<std::vector<char>>& enemyGrid,
        const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing, int difficulty, Enemy& enemy) {
        sf::RectangleShape selectionSquare(sf::Vector2f(cellSize, cellSize));
        selectionSquare.setFillColor(sf::Color::Transparent);
        selectionSquare.setOutlineThickness(2.0f);
        selectionSquare.setOutlineColor(sf::Color::Green);

        bool shotMade = false;

        sf::Event event;
        sf::Vector2i cursorPos(0, 0);
        sf::Vector2f manualOffset(645.0f, 0.0f);


        while (window.isOpen() && !shotMade) {
            while (window.pollEvent(event)) {
              
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                
                    sf::Vector2f relativePosition = static_cast<sf::Vector2f>(mousePosition) - (gridStartPosition + manualOffset);

                    float adjustedX = std::max(0.0f, std::min(relativePosition.x, static_cast<float>((cellSize + cellSpacing) * 9)));
                    float adjustedY = std::max(0.0f, std::min(relativePosition.y, static_cast<float>((cellSize + cellSpacing) * 9)));

                    int gridX = static_cast<int>(adjustedX / (cellSize + cellSpacing));
                    int gridY = static_cast<int>(adjustedY / (cellSize + cellSpacing));

                   
                    cursorPos.x = gridX;
                    cursorPos.y = gridY;
                }

                else if (event.type == sf::Event::KeyPressed) {
                    int gridSize = static_cast<int>(enemyGrid.size()); 

                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up) {
                        cursorPos.y = (cursorPos.y - 1 + gridSize) % gridSize;
                    }
                    else if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::Left) {
                        cursorPos.x = (cursorPos.x - 1 + gridSize) % gridSize;
                    }
                    else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down) {
                        cursorPos.y = (cursorPos.y + 1) % gridSize;
                    }
                    else if (event.key.code == sf::Keyboard::D || event.key.code == sf::Keyboard::Right) {
                        cursorPos.x = (cursorPos.x + 1) % gridSize;

                    }
                    if ((event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) &&
                        enemyGrid[cursorPos.x][cursorPos.y] != 'H' && enemyGrid[cursorPos.x][cursorPos.y] != 'M') {
                        char cellValue = enemyGrid[cursorPos.x][cursorPos.y]; 
                        if (cellValue == 'S') {
                            enemyGrid[cursorPos.x][cursorPos.y] = 'H'; 
                            enemy.updateEnemyGrid(cursorPos.x, cursorPos.y, 'H'); 
                        }
                        else if (cellValue == 'E') {
                            enemyGrid[cursorPos.x][cursorPos.y] = 'M'; 
                            enemy.updateEnemyGrid(cursorPos.x, cursorPos.y, 'M'); 
                        }
                        shotMade = true;
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if ((event.mouseButton.button == sf::Mouse::Left || event.mouseButton.button == sf::Mouse::Right) &&
                        enemyGrid[cursorPos.x][cursorPos.y] != 'H' && enemyGrid[cursorPos.x][cursorPos.y] != 'M') {
                        char cellValue = enemyGrid[cursorPos.x][cursorPos.y]; 
                        if (cellValue == 'S') {
                            enemyGrid[cursorPos.x][cursorPos.y] = 'H'; 
                            enemy.updateEnemyGrid(cursorPos.x, cursorPos.y, 'H'); 
                        }
                        else if (cellValue == 'E') {
                            enemyGrid[cursorPos.x][cursorPos.y] = 'M'; 
                            enemy.updateEnemyGrid(cursorPos.x, cursorPos.y, 'M'); 
                        }
                        
                        shotMade = true;
                    }
                }
            }
            

            window.clear(sf::Color::Black);
            selectionSquare.setPosition(gridStartPosition.x + 645 + cursorPos.x * (cellSize + cellSpacing), gridStartPosition.y + cursorPos.y * (cellSize + cellSpacing));
            window.draw(selectionSquare);

            renderMatchGrid(window, playerField, gridStartPosition, cellSize, cellSpacing);
            enemy.showEnemyGrid(window, enemy.getEnemyGrid(), gridStartPosition, cellSize, cellSpacing); // Draw the updated enemy's grid

            window.display();
        }
    }
    bool shipsLeft(const std::vector<std::vector<char>>& grid) {
        for (const auto& row : grid) {
            for (char cell : row) {
                if (cell == 'S') {
                    return true; 
                }
            }
        }
        return false;  
    }

    void createAndDrawText(sf::RenderWindow& window, const std::string& content, float x, float y, int fontSize, const sf::Color& color, const sf::Font& font) {
        sf::Text text(content, font, fontSize);

        text.setFont(font);

        text.setPosition(x, y);
        text.setFillColor(color);

      
        window.clear();
        window.draw(text);
        window.display();
    }


    void endMatch(sf::RenderWindow& window, int screen, sf::Font& font) {
        std::string text;
        InitializeState initState;
        initState.startall();

        switch (screen) {
        case 0:
            initState.getPlayerWinMusic().play();
            text = " You  won! ";
            break;
        case 1:
            initState.getEnemyWinMusic().play();
            text = "Enemy  won!";
            break;
        default:
            text = "Unknown result!";
            break;
        }

        float x = 425;
        float y = 300;


       


        window.clear();


        createAndDrawText(window, text, x, y, 100, sf::Color::White, font);

        sf::sleep(sf::seconds(10.0f));
        initState.getEnemyWinMusic().stop();
        initState.getPlayerWinMusic().stop();
        window.close();

    }



    void startMatch(std::vector<std::vector<char>>& playerField, sf::RenderWindow& window, sf::Font& fnt) {
        const sf::Vector2f gridStartPosition(55.0f, 200.0f);
        const float cellSize = 40.0f;
        const float cellSpacing = 5.0f;
   

        Enemy enemy;
        InitializeState ini;
        ini.startall();
        ini.getMatchMusic().setLoop(true);
        ini.getMatchMusic().play();
        enemy.generateEnemyGrid(); 
        int difficulty = chooseDifficulty(window);


        const int PLAYER_TURN = 0;
        const int ENEMY_TURN = 1;

        int turn = PLAYER_TURN; 

        while (true) {
            switch (turn) {
            case PLAYER_TURN:
                shooty(window, playerField, enemy.getEnemyGrid(), gridStartPosition, cellSize, cellSpacing, difficulty, enemy);
                sink(enemy.getEnemyGrid());

                ini.getGunfireMusic().play();
              
                if (!shipsLeft(enemy.getEnemyGrid())) {
                    ini.getGunfireMusic().stop();
                    ini.getMatchMusic().stop();
                    endMatch(window, 0, fnt);  
                    
                }
                sf::sleep(sf::seconds(1.0f));
                break;

            case ENEMY_TURN:
                enemy.enemyTurn(playerField, difficulty);
                enemy.showEnemyGrid(window, enemy.getEnemyGrid(), gridStartPosition, cellSize, cellSpacing);
                sink(playerField);
                if (!shipsLeft(playerField)) {
                    ini.getMatchMusic().stop();
                    ini.getGunfireMusic().stop();
                    endMatch(window, 1, fnt); 
                    
                }
                break;

            default:
            
                break;
            }

            // turn switching
            turn = (turn == PLAYER_TURN) ? ENEMY_TURN : PLAYER_TURN;
        }
    }
   
      
    

    
};

#endif // MATCHSTATE_H