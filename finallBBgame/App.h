#pragma once
#include <SFML/Graphics.hpp>
#include "InitializeState.h"
#include "MenuState.h"

#include <SFML/System.hpp>

/*
the "loading" screen when the game is launched
*/

void game() {
    InitializeState initState;
    initState.startall();
    initState.getIntroMusic().play();
    initState.startwindow();
   

    sf::RenderWindow& window = initState.getWindow();
    sf::Font& font = initState.getFont();

    sf::Texture& introTexture = initState.getIntroTexture();
    sf::Sprite introSprite(introTexture);

   
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        
        window.clear(sf::Color::Black);

      
        window.draw(introSprite);

     
        window.display();

        if (clock.getElapsedTime().asSeconds() >= 6.0f) { //timer so the whole song is played 
            break; 
        }
    }

    MenuState menuState(window, font);

    menuState.handleMenu();
}



