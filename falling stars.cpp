
#include "falling_stars.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>


void fallingStarsGame() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Falling Stars");

    // Star and "Game Over" object
    sf::CircleShape star(10);
    star.setFillColor(sf::Color(
        std::rand() % 256, std::rand() % 256, std::rand() % 256
    ));

    sf::RectangleShape gameOverObject(sf::Vector2f(20.0, 20.0));
    gameOverObject.setFillColor(sf::Color::Red);

    sf::RectangleShape catcher(sf::Vector2f(160.0, 20.0));
    catcher.setFillColor(sf::Color::Blue);
    catcher.setPosition(350, 550);

    sf::Font font;
    if (!font.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/Orbitron-Regular.ttf")) {
        std::cerr << "Error loading font.\n";
        return;
    }

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setString("You Died :( ");
    gameOverText.setPosition(370, 320);

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    float starX = std::rand() % 800;
    float starY = 0;

    float gameOverX = std::rand() % 800;
    float gameOverY = -200; // Start off-screen

    int score = 0;
    float speed = 0.8f; // Initial speed
    bool isGameOver = false;

    sf::Clock clock; // To manage speed increases over time
    sf::Clock movementClock;  // To track time for smooth movement
    float deltaTime = 0.0f;   // To store time difference for smooth movement
    float catcherSpeed = 400.0f; // Catcher speed in pixels per second

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Restart the game after death
            if (isGameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                score = 0;
                speed = 0.5f;
                starY = 0;
                gameOverY = -200;
                isGameOver = false;
            }
        }

        if (!isGameOver) {
            // Get the time elapsed since the last frame
            deltaTime = movementClock.restart().asSeconds();

            // Move catcher based on deltaTime
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && catcher.getPosition().x > 0)
                catcher.move(-catcherSpeed * deltaTime, 0);  // Move left
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && catcher.getPosition().x < window.getSize().x - catcher.getSize().x)
                catcher.move(catcherSpeed * deltaTime, 0);  // Move right

            // Update speed dynamically based on time
            if (clock.getElapsedTime().asSeconds() > 15.0f) { // Increase speed every 15 seconds
                speed += 0.07f; // Gradual speed increase
                clock.restart();
            }

            // Update star position
            starY += speed;
            if (starY > 600) {
                starY = 0;
                starX = std::rand() % 800;

                // Change star to a random color
                star.setFillColor(sf::Color(
                    std::rand() % 256, std::rand() % 256, std::rand() % 256
                ));
            }

            // Update game-over object position
            gameOverY += speed * 0.8f; // Slightly slower than the star
            if (gameOverY > 600) {
                gameOverY = -200; // Reset off-screen
                gameOverX = std::rand() % 800;
            }

            // Collision detection
            if (star.getGlobalBounds().intersects(catcher.getGlobalBounds())) {
                score++;
                starY = 0;
                starX = std::rand() % 800;
            }

            if (gameOverObject.getGlobalBounds().intersects(catcher.getGlobalBounds())) {
                isGameOver = true;
            }

            // Update score text
            scoreText.setString("Score: " + std::to_string(score) + "  Speed: " + std::to_string(static_cast<int>(speed)));
        }

        // Draw everything
        window.clear();
        if (!isGameOver) {
            star.setPosition(starX, starY);
            gameOverObject.setPosition(gameOverX, gameOverY);

            window.draw(star);
            window.draw(gameOverObject);
            window.draw(catcher);
            window.draw(scoreText);
        }
        else {
            window.draw(gameOverText);
        }
        window.display();
    }
}
