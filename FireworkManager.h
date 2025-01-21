
#ifndef FIREWORK_MANAGER_H
#define FIREWORK_MANAGER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Firework.h"

class FireworkManager {
private:
    std::vector<Firework> fireworks;
    sf::RenderWindow& window;

public:
    FireworkManager(sf::RenderWindow& win) : window(win) {
        std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed random generator
    }

    void update() {
        // Add new fireworks if fewer than 5
        if (fireworks.size() < 5) {
            fireworks.push_back(Firework(window));
        }

        // Update and draw each firework
        for (auto it = fireworks.begin(); it != fireworks.end();) {
            it->update(window);
            if (it->isFinished()) {
                it = fireworks.erase(it); // Remove finished fireworks
            } else {
                ++it;
            }
        }
    }

    void reset() {
        fireworks.clear(); // Clear all fireworks
    }
};

#endif
