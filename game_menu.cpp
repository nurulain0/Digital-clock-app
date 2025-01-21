#include "game_menu.h"
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

void gameMenu(RenderWindow& window, GameState& currentState) {
    // Menu loop
    while (window.isOpen() && currentState == GameState::MENU) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // Handle the "G" button logic
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::G) {
                // Ask the user which game they want to play via the console
                cout << "Choose a game:\n";
                cout << "1. Falling Stars\n";
                cout << "2. Arkanoid\n";
                cout << "Enter the number of the game you want to play: ";

                int choice;
                cin >> choice;

                if (choice == 1) {
                    currentState = GameState::FALLING_STARS;
                    return;
                }
                else if (choice == 2) {
                    currentState = GameState::ARKANOID;
                    return;
                }
                else {
                    cout << "Invalid choice. Returning to menu.\n";
                }
            }
        }

        // Keep the existing graphics as-is
        window.clear();
        // Any existing menu graphics (already in your source.cpp) will be redrawn here.
        window.display();
    }
}
