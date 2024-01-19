#ifndef ENEMY_H
#define ENEMY_H

#include <vector>
#include <string>
#include <cstdlib> // for rand and srand
#include <ctime> // For time

class Enemy {


public:
    Enemy() {
        srand(static_cast<unsigned int>(time(0)));  
        generateEnemyGrid(); 
    }

    void generateEnemyGrid();

    void showEnemyGrid(sf::RenderWindow& window, const std::vector<std::vector<char>>& enemyField, const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing);

    void enemyTurn(std::vector<std::vector<char>>& playerField, int difficulty);


    std::vector<std::vector<char>>& getEnemyGrid();

    std::vector<std::vector<char>> enemyGrid;
   
    void updateEnemyGrid(int x, int y, char result) {
        enemyGrid[x][y] = result;
    }

};

void Enemy::generateEnemyGrid() {

    enemyGrid = std::vector<std::vector<char>>(10, std::vector<char>(10, 'E'));

    std::vector<int> shipLengths = { 4, 3, 2, 2, 1, 1, 1, 1 }; // Ship lengths

    for (int length : shipLengths) {
        bool placed = false;
        while (!placed) {
   
            int x = rand() % 10;
            int y = rand() % 10; 

            
            bool isVertical = rand() % 2 == 0; 
            bool validPlacement = true;
            if (isVertical) {
                if (x + length > 10) {
                    validPlacement = false;
                }
                else {

                    for (int i = std::max(0, x - 1); i < std::min(10, x + length + 1); ++i) {
                        if (y > 0 && enemyGrid[i][y - 1] == 'S') {
                            validPlacement = false;
                            break;
                        }
                        if (y < 9 && enemyGrid[i][y + 1] == 'S') {
                            validPlacement = false;
                            break;
                        }
                    }
                }
            }
            else {
                if (y + length > 10) {
                    validPlacement = false;
                }
                else {
               
                    for (int i = std::max(0, y - 1); i < std::min(10, y + length + 1); ++i) {
                        if (x > 0 && enemyGrid[x - 1][i] == 'S') {
                            validPlacement = false;
                            break;
                        }
                        if (x < 9 && enemyGrid[x + 1][i] == 'S') {
                            validPlacement = false;
                            break;
                        }
                    }
                }
            }

            if (validPlacement) {
                if (isVertical) {
                    for (int i = x; i < x + length; ++i) {
                        enemyGrid[i][y] = 'S'; 
                    }
                }
                else {
                    for (int i = y; i < y + length; ++i) {
                        enemyGrid[x][i] = 'S'; 
                    }
                }
                placed = true; 
            }
        }
    }
}


void Enemy::showEnemyGrid(sf::RenderWindow& window, const std::vector<std::vector<char>>& enemyField, const sf::Vector2f& gridStartPosition, float cellSize, float cellSpacing) {
    for (size_t i = 0; i < enemyField.size(); ++i) {
        for (size_t j = 0; j < enemyField[i].size(); ++j) {
            sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
            cell.setPosition(gridStartPosition.x +645+ i * (cellSize + cellSpacing), gridStartPosition.y + j * (cellSize + cellSpacing));
            if (enemyField[i][j] == 'E'|| enemyField[i][j] == 'S') {
                cell.setFillColor(sf::Color::Blue);
            }
            else if (enemyField[i][j] == 'H') {
                cell.setFillColor(sf::Color::Red);
            }
            else if (enemyField[i][j] == 'M') {
                cell.setFillColor(sf::Color(0, 150, 150, 255));
            }
            
            window.draw(cell);
        }
    }
}
std::vector<std::vector<char>>& Enemy::getEnemyGrid() {
    return enemyGrid;
}

void mediumAI(std::vector<std::vector<char>>& playerField, int& targetX, int& targetY) {
    int fieldSize = playerField.size();
    bool validTarget = false;


    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            if (playerField[i][j] == 'H') {
                
                bool blocked = true;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        int ni = i + di;
                        int nj = j + dj;

        
                        if (ni >= 0 && ni < fieldSize && nj >= 0 && nj < fieldSize) {
                            if (playerField[ni][nj] != 'H' && playerField[ni][nj] != 'M') {
                                blocked = false;
                                break;
                            }
                        }
                    }
                    if (!blocked) {
                        break;
                    }
                }

                if (!blocked) {
                    std::vector<int> unblockedX;
                    std::vector<int> unblockedY;

                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            int ni = i + di;
                            int nj = j + dj;

                            if (ni >= 0 && ni < fieldSize && nj >= 0 && nj < fieldSize &&
                                playerField[ni][nj] != 'H' && playerField[ni][nj] != 'M') {
                                unblockedX.push_back(ni);
                                unblockedY.push_back(nj);
                            }
                        }
                    }

                    if (!unblockedX.empty() && !unblockedY.empty()) {
                        int randomIndex = rand() % unblockedX.size();
                        targetX = unblockedX[randomIndex];
                        targetY = unblockedY[randomIndex];
                        validTarget = true;
                        break;
                    }
                }
            }
        }
        if (validTarget) {
            break;
        }
    }

    if (!validTarget) {
        while (!validTarget) {
            targetX = rand() % fieldSize;
            targetY = rand() % fieldSize;

            if (playerField[targetX][targetY] != 'H' && playerField[targetX][targetY] != 'M') {
                validTarget = true;
            }
        }
    }

    if (playerField[targetX][targetY] == 'S') {
        playerField[targetX][targetY] = 'H';
    }
    else {
        playerField[targetX][targetY] = 'M';
        
    }
}



void hardAI(std::vector<std::vector<char>>& playerField, int& targetX, int& targetY) {
    int fieldSize = playerField.size();
    bool validTarget = false;

    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            if (playerField[i][j] == 'H') {
           
                bool blocked = true;
                for (int di = -1; di <= 1; ++di) {
                    for (int dj = -1; dj <= 1; ++dj) {
                        int ni = i + di;
                        int nj = j + dj;

                        if (ni >= 0 && ni < fieldSize && nj >= 0 && nj < fieldSize) {
                            if (playerField[ni][nj] != 'H' && playerField[ni][nj] != 'M') {
                                blocked = false;
                                break;
                            }
                        }
                    }
                    if (!blocked) {
                        break;
                    }
                }

              
                if (!blocked) {
                    std::vector<int> unblockedX;
                    std::vector<int> unblockedY;

                    for (int di = -1; di <= 1; ++di) {
                        for (int dj = -1; dj <= 1; ++dj) {
                            int ni = i + di;
                            int nj = j + dj;


                            if (ni >= 0 && ni < fieldSize && nj >= 0 && nj < fieldSize &&
                                playerField[ni][nj] != 'H' && playerField[ni][nj] != 'M') {
                                unblockedX.push_back(ni);
                                unblockedY.push_back(nj);
                            }
                        }
                    }

                    if (!unblockedX.empty() && !unblockedY.empty()) {
                        int randomIndex = rand() % unblockedX.size();
                        targetX = unblockedX[randomIndex];
                        targetY = unblockedY[randomIndex];
                        validTarget = true;
                        break;
                    }
                }
            }
        }
        if (validTarget) {
            break;
        }
    }

    if (!validTarget && rand() % 2 == 0) {

        do {
            targetX = rand() % fieldSize;
            targetY = rand() % fieldSize;
        } while (playerField[targetX][targetY] != 'S');

        validTarget = true;
    }

    if (!validTarget) {
        while (!validTarget) {
            targetX = rand() % fieldSize;
            targetY = rand() % fieldSize;

           
            if (playerField[targetX][targetY] != 'H' && playerField[targetX][targetY] != 'M') {
                validTarget = true;
            }
        }
    }
    if (playerField[targetX][targetY] == 'S') {
        playerField[targetX][targetY] = 'H';
    }
    else {
        playerField[targetX][targetY] = 'M';

    }
}



void Enemy::enemyTurn(std::vector<std::vector<char>>& playerField, int difficulty) {
    int fieldSize = playerField.size();

    bool validTarget = false;
    int targetX = 0, targetY = 0;

   
    
    switch (difficulty) {
    case 0:
        while (!validTarget) {
            targetX = rand() % fieldSize;
            targetY = rand() % fieldSize;

            if (playerField[targetX][targetY] != 'H' && playerField[targetX][targetY] != 'M') {
                validTarget = true;
            }
        }
       
        if (playerField[targetX][targetY] == 'S') {
            playerField[targetX][targetY] = 'H';
        }
        else {
            playerField[targetX][targetY] = 'M'; 
        }
        break;

    case 1:
        mediumAI(playerField, targetX, targetY);
        break;

    case 2:
        hardAI(playerField, targetX, targetY);
        break;

    default:

        if (playerField[targetX][targetY] == 'S') {
            playerField[targetX][targetY] = 'H';
        }
        else {
            playerField[targetX][targetY] = 'M';

        }
        break;
    }
}





#endif // ENEMY_H