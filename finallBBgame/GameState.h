#ifndef GAMESTATE_H
#define GAMESTATE_H

/*

in the end not used

but allows quite interesting switching of the application and assets from one state to another, 

if implemented correctly can make application smoother as it limits amount of the code/assets that need to be loaded
*/

enum class GameState {
    INITIALIZE, //loads the window and graphical elements
    MENU, // gives plazers an option to choose a singleplazer game, help, or end the app
    SETUP,// game itself, 
    MATCH

};


#endif // GAMESTATE_H



// window.clear(sf::Color::Black);