#ifndef INITIALIZESTATE_H //prevents multiple initialisation, #problem saver
#define INITIALIZESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/*
class used to get all the textures, music and fonts, 

the start all in combination iwth getters allows to obtain ANY items we might need from this class
*/
class InitializeState {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture easy;
    sf::Texture medium;
    sf::Texture hard;
    sf::Texture intro;
    sf::Texture menu;
    sf::Music musicintro;
    sf::Music musicmenu;
    sf::Music setup;
    sf::Music match;
    sf::Music gunfire;
    sf::Music playerW;
    sf::Music enemyW;
 

   

public:
    InitializeState() {
    }
    void startwindow() {
        window.create(sf::VideoMode(1200, 800), "Battleship Game");
    }
    void startall() {
        enemyW.openFromFile("graphics/aiwin.mp3");
        playerW.openFromFile("graphics/playerwin.mp3");
        setup.openFromFile("graphics/shipsetup.mp3");
        match.openFromFile("graphics/matchrain.mp3");
        gunfire.openFromFile("graphics/salvo.mp3");
        musicmenu.openFromFile("graphics/menumusic.mp3");
        musicintro.openFromFile("graphics/intromusic.mp3");
        font.loadFromFile("graphics/ChelaOne-Regular.ttf");
        easy.loadFromFile("graphics/easy.png");
        medium.loadFromFile("graphics/medium.png");
        hard.loadFromFile("graphics/hard.png");
        intro.loadFromFile("graphics/intro.png");
        menu.loadFromFile("graphics/menu.png");
       
    }

    void openWindow() {
        // Display the window
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear(sf::Color::Black);

    

            window.display();
        }
    }

    sf::RenderWindow& getWindow() {
        return window;
    }

    sf::Font& getFont() {
        return font;
    }

    sf::Texture& getEasyTexture() {
        return easy;
    }

    sf::Texture& getMediumTexture() {
        return medium;
    }

    sf::Texture& getHardTexture() {
        return hard;
    }

    sf::Texture& getIntroTexture() {
        return intro;
    }

    sf::Texture& getMenuTexture() {
        return menu;
    }
    sf::Music& getIntroMusic() {
        return musicintro;
    }
    sf::Music& getMenuMusic() {
        return musicmenu;
    }
    sf::Music& getSetupMusic() {
        return setup;
    }


    sf::Music& getMatchMusic() {
        return match;
    }

    sf::Music& getGunfireMusic() {
        return gunfire;
    }
    sf::Music& getPlayerWinMusic() {
        return playerW;
    }

    sf::Music& getEnemyWinMusic() {
        return enemyW;
    }
};

#endif // INITIALIZESTATE_H
