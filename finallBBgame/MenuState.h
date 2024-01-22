#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <SFML/Graphics.hpp>
#include "setupState.h"
#include "App.h"

/*
menu screenm 

exit and help handling
*/

//kills game
void endGame() {

    exit(EXIT_SUCCESS); 
}


//displays help

//left when mouse or enter is pressed
void displayHelp(sf::RenderWindow& window, sf::Font& font) {
    std::vector<std::pair<std::string, std::string>> helpSections = {
        {"BASICS", "This game is about two fleets clashing against each other\n           the one to sink all enemy ships first wins."},
        {"CONTROLS", "WASD or arrow keys for control,\n           SPACEBAR to rotate,\n              ENTER to confirm."},
        {"DISCLAIMER", "           This game is just a school project,\n and is meant to be a satire of current situation."}
        // Add more help sections as needed
    };

    sf::Text titleText("", font, 40); // Title font size
    titleText.setPosition(100, 100); // Adjust position as needed

    sf::Text descriptionText("", font, 30); // Regular text font size
    descriptionText.setPosition(100, 150); // Adjust position as needed

   

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                // Return to the menu on any mouse click
                return;
            }
            else if (event.type == sf::Event::KeyPressed) {
                // Handle navigation between sections or back to menu
                // Example:
                if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space) {
                    return;
                }

            }
        }

        window.clear();

        // Display all help sections
        for (const auto& section : helpSections) {
            titleText.setString(section.first);// vector split
            descriptionText.setString(section.second);

            // Center horizontally
            float titleWidth = titleText.getLocalBounds().width;
            float descriptionWidth = descriptionText.getLocalBounds().width;
            titleText.setPosition((window.getSize().x - titleWidth) / 2, titleText.getPosition().y);
            descriptionText.setPosition((window.getSize().x - descriptionWidth) / 2, descriptionText.getPosition().y);

            window.draw(titleText);
            window.draw(descriptionText);

           
            titleText.setPosition(100, titleText.getPosition().y + 200); 
            descriptionText.setPosition(100, descriptionText.getPosition().y + 200);
        }

        
        titleText.setPosition(100, 100);
        descriptionText.setPosition(100, 150);

        window.display();
    }
}

class MenuState {
private://menu
    sf::RenderWindow& window;
    sf::Font& font;
    int selectedOption;

public:
    MenuState(sf::RenderWindow& win, sf::Font& fnt) : window(win), font(fnt), selectedOption(0) {}
    SetupState setupState;
    InitializeState ini;
    void handleMenu() {
        
        ini.startall();
        sf::Texture& menutexture = ini.getMenuTexture();
        sf::Sprite menusprite(menutexture);
        
        ini.getMenuMusic().setLoop(true);

        // Play the menu music
        ini.getMenuMusic().play();
       
        //all texts in a single vector, making them easier to manage, centred via spaces :D
       
        std::vector<std::string> menuOptions = { "Sink  them all!", "       Help   ", "       Quit   " };
       
        
        //fucking moooouse => clickable areas 
        struct ClickableArea {
            sf::FloatRect bounds;
            int option;
        }; 

        std::vector<ClickableArea> clickableAreas;
        float verticalSpacing = 70.0f; 

        for (size_t i = 0; i < menuOptions.size(); ++i) {
            sf::Text menuText(menuOptions[i], font);
            menuText.setCharacterSize(30);
            sf::FloatRect textBounds = menuText.getGlobalBounds();

            
            sf::Vector2f textPosition((window.getSize().x - textBounds.width) / 2,
                (window.getSize().y - textBounds.height * menuOptions.size()) / 2 +
                i * (textBounds.height + verticalSpacing) - 100);

            ClickableArea clickableArea;
            clickableArea.bounds = sf::FloatRect(textPosition, sf::Vector2f(textBounds.width, textBounds.height));
            clickableArea.option = static_cast<int>(i);
            clickableAreas.push_back(clickableArea);
        }

        //controls handling

        while (window.isOpen()) {
            sf::Event event;

         
   
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                else if (event.type == sf::Event::MouseMoved) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

                    for (const ClickableArea& clickableArea : clickableAreas) {
                        if (clickableArea.bounds.contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                            selectedOption = clickableArea.option;
                        }
                    }
                }

                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (const ClickableArea& clickableArea : clickableAreas) {
                            if (clickableArea.bounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                                // Handle the click based on the option
                                if (clickableArea.option == 0) {
                                    ini.getMenuMusic().stop();
                                    setupState.setup(window, font);
                                }
                                else if (clickableArea.option == 1) {
                                    ini.getMenuMusic().stop();
                                    displayHelp(window, font);
                                }
                                else if (clickableArea.option == 2) {
                                    ini.getMenuMusic().stop();
                                    endGame();
                                }
                            }
                        }
                    }
                }
            
                else if (event.type == sf::Event::KeyPressed) { //keyboard
                    if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
                        selectedOption = (selectedOption + menuOptions.size() - 1) % menuOptions.size();
                    }
                    else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
                        selectedOption = (selectedOption + 1) % menuOptions.size();
                    }
                    else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
                        if (selectedOption == 0) {
                            ini.getMenuMusic().stop();
                            setupState.setup(window, font);
                        }
                        else if (selectedOption == 1) {
                            ini.getMenuMusic().stop();
                            displayHelp(window, font);
                        }
                        else if (selectedOption == 2) {
                            ini.getMenuMusic().stop();
                            endGame();
                        }
                    }
                }
            }
            window.clear(sf::Color::Black);

            
            window.draw(menusprite);
          

            // Calculate center position for menu text
            sf::Vector2u windowSize = window.getSize();
            sf::Vector2f textBounds = sf::Text(menuOptions[0], font, 30).getGlobalBounds().getSize();
            float spacing = 70.0f; 
            sf::Vector2f textPosition((windowSize.x - textBounds.x) / 2, (windowSize.y - (textBounds.y + spacing) * menuOptions.size()) / 2);
            // collor switching 
            for (size_t i = 0; i < menuOptions.size(); ++i) {
                sf::Text menuText(menuOptions[i], font);
                menuText.setCharacterSize(30);
                menuText.setFillColor((i == selectedOption) ? sf::Color::Yellow : sf::Color::White);
                menuText.setPosition(textPosition.x, textPosition.y + i * (textBounds.y + spacing)); 
                window.draw(menuText);                 
            }
           
            window.display();
        }
    }
};

#endif // MENUSTATE_H