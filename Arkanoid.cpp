#include "arkanoid.h"
#include <SFML/Graphics.hpp>
#include <ctime>
#include <cstdlib>

void arkanoidGame() {
    // Seed for random numbers
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Create a render window
    sf::RenderWindow app(sf::VideoMode(520, 450), "Arkanoid!");
    app.setFramerateLimit(60);

    // Load textures
    sf::Texture t1, t2, t3, t4;
    if (!t1.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/images/block01.png") ||
        !t2.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/images/background.jpg") ||
        !t3.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/images/ball.png") ||
        !t4.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/images/paddle.png")) {
        return; // Exit if any texture fails to load
    }

    // Create sprites
    sf::Sprite sBackground(t2), sBall(t3), sPaddle(t4);
    sPaddle.setPosition(300, 440);

    // Initialize blocks
    sf::Sprite block[1000];
    int n = 0;
    for (int i = 1; i <= 10; i++) {
        for (int j = 1; j <= 10; j++) {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }
    }

    // Ball and paddle movement variables
    float dx = 6.f, dy = 5.f;
    float x = 300.f, y = 300.f;

    // Game loop
    while (app.isOpen()) {
        sf::Event e;
        while (app.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                app.close();
        }

        // Ball movement
        x += dx;
        for (int i = 0; i < n; i++) {
            if (sf::FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                block[i].setPosition(-100, 0); // Move block out of view
                dx = -dx;                     // Reflect ball horizontally
            }
        }

        y += dy;
        for (int i = 0; i < n; i++) {
            if (sf::FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds())) {
                block[i].setPosition(-100, 0); // Move block out of view
                dy = -dy;                     // Reflect ball vertically
            }
        }

        // Wall collision
        if (x < 0 || x > 520) dx = -dx;
        if (y < 0 || y > 450) dy = -dy;

        // Paddle movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sPaddle.getPosition().x + sPaddle.getGlobalBounds().width < 520) {
            sPaddle.move(6, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sPaddle.getPosition().x > 0) {
            sPaddle.move(-6, 0);
        }

        // Paddle collision
        if (sf::FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) {
            dy = -(std::rand() % 5 + 2); // Reflect ball with random speed
        }

        // Update ball position
        sBall.setPosition(x, y);

        // Draw everything
        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);
        for (int i = 0; i < n; i++) {
            app.draw(block[i]);
        }
        app.display();
    }
}
