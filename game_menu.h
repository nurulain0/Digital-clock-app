#ifndef GAME_MENU_H
#define GAME_MENU_H

#include <SFML/Graphics.hpp>
#include "falling_stars.h"
#include "arkanoid.h"

// Enum to manage game states
enum class GameState {
    MENU,
    FALLING_STARS,
    ARKANOID,
    EXIT
};

// Function prototype for the game menu
void gameMenu(sf::RenderWindow& window, GameState& currentState);

#endif // GAME_MENU_H
