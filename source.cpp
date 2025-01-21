#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include "timeZone.h"
#include "reminder.h"  // Include the reminder functionality header
#include "FireworkManager.h"
#include "falling_stars.h" // Include the falling stars game header
#include "arkanoid.h"      // Include the arkanoid game header

using namespace std;

// Global reminder vector (declared in reminder.h)
extern std::vector<Reminder> reminders;

// Function to create a plain black background
sf::VertexArray createPlainBackground(const sf::Vector2u& windowSize) {
    sf::VertexArray background(sf::Quads, 4);

    background[0].position = sf::Vector2f(0, 0);
    background[1].position = sf::Vector2f(windowSize.x, 0);
    background[2].position = sf::Vector2f(windowSize.x, windowSize.y);
    background[3].position = sf::Vector2f(0, windowSize.y);

    // Set background color to black
    background[0].color = sf::Color::Black;
    background[1].color = sf::Color::Black;
    background[2].color = sf::Color::Black;
    background[3].color = sf::Color::Black;

    return background;
}

// Function to update the digital clock text
void updateDigitalClock(sf::Text& text, sf::Text& amPmText) {
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%I:%M", &localTime_s); // Time part

    text.setString(buffer);

    // Set AM/PM
    std::string amPm = (localTime_s.tm_hour >= 12) ? "PM" : "AM";
    amPmText.setString(amPm);

    // Adjust AM/PM position based on the digital clock's width
    float offsetX = text.getGlobalBounds().width + 10; // 10px spacing after the time part
    amPmText.setPosition(text.getPosition().x + offsetX, text.getPosition().y + 5); // +5 for slight adjustment
}

// Function to update the analog clock
void updateAnalogClock(sf::RectangleShape& hourHand, sf::RectangleShape& minuteHand, sf::RectangleShape& secondHand) {
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    // Calculate the angles based on the current time
    float hourAngle = (localTime_s.tm_hour % 12) * 30.f; // 360 / 12 = 30 degrees per hour
    float minuteAngle = localTime_s.tm_min * 6.f; // 360 / 60 = 6 degrees per minute
    float secondAngle = localTime_s.tm_sec * 6.f; // 360 / 60 = 6 degrees per second

    // Set the rotation for each hand (clockwise)
    hourHand.setRotation(90 + (hourAngle + (localTime_s.tm_min / 2.0f))); // Adjust for minutes in hour
    minuteHand.setRotation(90 + minuteAngle); // Correct minute hand rotation
    secondHand.setRotation(secondAngle);  // Correct second hand rotation
}

// Function to handle game selection logic via "G" button
void handleGameSelection(sf::RenderWindow& window) {
    int chosenGame = 0;
    cout << "Which game would you like to play?" << endl;
    cout << "1. Falling Stars" << endl;
    cout << "2. Arkanoid" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> chosenGame;

    if (chosenGame == 1) {
        fallingStarsGame(); // Call the Falling Stars game
    }
    else if (chosenGame == 2) {
        arkanoidGame(); // Call the Arkanoid game
    }
    else {
        cout << "Invalid choice, returning to menu." << endl;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Clock UI");
    window.setFramerateLimit(60);

    // Ask the user for a time zone and get the offset
    int timeZoneOffset = selectTimeZone();  // Get the time zone offset based on user input

    // Ask user if they want to enable dynamic background
    cout << "Would you like to enable dynamic background? (yes/no): ";
    string userInput;
    cin >> userInput;

    bool enableDynamicBackground = (userInput == "yes");

    // Firework Manager
    FireworkManager fireworkManager(window);
    bool showFireworks = false; // Fireworks toggle

    // Plain black background
    auto background = createPlainBackground(window.getSize());

    // Font setup
    sf::Font font;
    if (!font.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/DSEG7Classic-Regular.ttf")) {
        return -1;
    }
    // Font for the button text
    sf::Font buttonFont;
    if (!buttonFont.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/Mulish-SemiBold.ttf")) {
        return -1;
    }

    // Digital clock
    sf::Text digitalClock;
    digitalClock.setFont(font);
    digitalClock.setCharacterSize(68); // Smaller size
    digitalClock.setFillColor(sf::Color::White);
    digitalClock.setPosition(90, 220); // Moved higher

    // AM/PM label
    sf::Text amPmText;
    amPmText.setFont(font);
    amPmText.setCharacterSize(20); // Smaller size
    amPmText.setFillColor(sf::Color::White);
    amPmText.setPosition(digitalClock.getPosition().x + digitalClock.getGlobalBounds().width + 14, 224); // Positioned right next to time

    // Analog clock
    sf::CircleShape clockFace(100);
    clockFace.setFillColor(sf::Color(255, 255, 255));
    clockFace.setOutlineColor(sf::Color(0, 0, 139)); // Dark blue outline (RGB values for Dark Blue)
    clockFace.setOutlineThickness(2.5); // Thin border 
    clockFace.setOrigin(100, 100);
    clockFace.setPosition(550, 220); // Moved higher

    sf::RectangleShape hourHand(sf::Vector2f(50, 6));
    hourHand.setFillColor(sf::Color::Black);
    hourHand.setOrigin(50, 3);  // Origin to the center of the clock
    hourHand.setPosition(550, 220);  // Center of clock face

    sf::RectangleShape minuteHand(sf::Vector2f(70, 4));
    minuteHand.setFillColor(sf::Color::Black);
    minuteHand.setOrigin(70, 2);  // Origin to the center of the clock
    minuteHand.setPosition(550, 220);  // Center of clock face

    sf::RectangleShape secondHand(sf::Vector2f(90, 2));
    secondHand.setFillColor(sf::Color::Red);
    secondHand.setOrigin(90, 1);  // Origin to the center of the clock
    secondHand.setPosition(550, 220);  // Center of clock face

    // Square Buttons
    sf::RectangleShape buttons[3];
    sf::Text buttonText[3];

    std::string labels[3] = { "R", "F", "G" };

    for (int i = 0; i < 3; ++i) {
        // Button appearance (square)
        buttons[i].setSize(sf::Vector2f(60, 60)); // Square size
        buttons[i].setFillColor(sf::Color(0, 0, 139)); // Dark blue
        buttons[i].setOrigin(30, 30);
        buttons[i].setPosition(300 + i * 100, 450);

        // Button text
        buttonText[i].setFont(buttonFont);
        buttonText[i].setString(labels[i]);
        buttonText[i].setCharacterSize(25); // Adjust text size
        buttonText[i].setFillColor(sf::Color::White);
        sf::FloatRect textBounds = buttonText[i].getLocalBounds();
        buttonText[i].setOrigin(textBounds.width / 2, textBounds.height / 2);
        buttonText[i].setPosition(buttons[i].getPosition().x, buttons[i].getPosition().y + 11);
    }

    // Time-based color logic
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    // Default colors for night
    sf::Color backgroundColor = sf::Color::Black;
    sf::Color buttonColor = sf::Color(0, 0, 139);  // Dark Blue
    sf::Color outlineColor = sf::Color(0, 0, 139); // Dark Blue

    // Morning (6 AM - 12 PM)
    if (localTime_s.tm_hour >= 6 && localTime_s.tm_hour < 12) {
        backgroundColor = sf::Color(173, 216, 230);  // Light Blue
        buttonColor = sf::Color(255, 255, 102);      // Light Yellow
        outlineColor = sf::Color(255, 255, 102);     // Light Yellow
    }
    // Evening (12 PM - 6 PM)
    else if (localTime_s.tm_hour >= 12 && localTime_s.tm_hour < 18) {
        backgroundColor = sf::Color(139, 69, 19);    // Brown
        buttonColor = sf::Color(0, 100, 0);          // Dark Green
        outlineColor = sf::Color(0, 100, 0);         // Dark Green
    }

    if (enableDynamicBackground) {
        // Update colors if dynamic background is enabled
        background[0].color = backgroundColor;
        background[1].color = backgroundColor;
        background[2].color = backgroundColor;
        background[3].color = backgroundColor;

        // Update button and clock face outline colors
        for (int i = 0; i < 3; ++i) {
            buttons[i].setFillColor(buttonColor);
        }
        clockFace.setOutlineColor(outlineColor);
    }



    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 3; ++i) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        cout << labels[i] << " Button clicked!" << endl;

                        if (labels[i] == "R") {
                            addReminder();  // Add reminder functionality when "R" is clicked
                        }
                        if (labels[i] == "F") {
                            showFireworks = true;  // Enable fireworks
                        }
                        if (labels[i] == "G") {
                            // Trigger the game selection logic when the "G" button is clicked
                            handleGameSelection(window);
                        }
                    }
                }
            }


            // Fetch the current time based on the selected time zone
            std::tm localTime_s = getTimeForTimeZone(timeZoneOffset);

            // Update the dynamic background based on the hour
            if (enableDynamicBackground) {
                if (localTime_s.tm_hour >= 6 && localTime_s.tm_hour < 12) {
                    backgroundColor = sf::Color(173, 216, 230);  // Light Blue
                    buttonColor = sf::Color(255, 255, 102);      // Light Yellow
                    outlineColor = sf::Color(255, 255, 102);     // Light Yellow
                }
                else if (localTime_s.tm_hour >= 12 && localTime_s.tm_hour < 18) {
                    backgroundColor = sf::Color(139, 69, 19);    // Brown
                    buttonColor = sf::Color(0, 100, 0);          // Dark Green
                    outlineColor = sf::Color(0, 100, 0);         // Dark Green
                }
                else {
                    backgroundColor = sf::Color::Black;          // Default night colors
                    buttonColor = sf::Color(0, 0, 139);          // Dark Blue
                    outlineColor = sf::Color(0, 0, 139);         // Dark Blue
                }

                // Apply the background color
                background[0].color = backgroundColor;
                background[1].color = backgroundColor;
                background[2].color = backgroundColor;
                background[3].color = backgroundColor;

                // Update button and clock face outline colors
                for (int i = 0; i < 3; ++i) {
                    buttons[i].setFillColor(buttonColor);
                }
                clockFace.setOutlineColor(outlineColor);
            }
            // Update digital and analog clocks
            updateDigitalClock(digitalClock, amPmText, timeZoneOffset);
            updateAnalogClock(hourHand, minuteHand, secondHand, timeZoneOffset);

            // Check reminders every frame
            checkReminders();  // Check and display any reminders that should be triggered

            // Render everything
            window.clear();
            window.draw(background); // Draw the updated background
            window.draw(digitalClock);
            window.draw(amPmText);
            window.draw(clockFace);
            window.draw(hourHand);
            window.draw(minuteHand);
            window.draw(secondHand);
            for (int i = 0; i < 3; ++i) {
                window.draw(buttons[i]);
                window.draw(buttonText[i]);
            }
            // Draw fireworks if enabled
            if (showFireworks) {
                fireworkManager.update();
            }

            window.display();
        }
    }

    return 0;
}



/*
WORKING MAINCODE, ANDDDDDDDDD FIREWORKS, WHEN U CLICK SECOND BUTTON :




#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>
#include "timeZone.h"
#include "reminder.h"  // Include the reminder functionality header
#include "FireworkManager.h"


using namespace std;

// Global reminder vector (declared in reminder.h)
extern std::vector<Reminder> reminders;

// Function to create a plain black background
sf::VertexArray createPlainBackground(const sf::Vector2u& windowSize) {
    sf::VertexArray background(sf::Quads, 4);

    background[0].position = sf::Vector2f(0, 0);
    background[1].position = sf::Vector2f(windowSize.x, 0);
    background[2].position = sf::Vector2f(windowSize.x, windowSize.y);
    background[3].position = sf::Vector2f(0, windowSize.y);

    // Set background color to black
    background[0].color = sf::Color::Black;
    background[1].color = sf::Color::Black;
    background[2].color = sf::Color::Black;
    background[3].color = sf::Color::Black;

    return background;
}

// Function to update the digital clock text
void updateDigitalClock(sf::Text& text, sf::Text& amPmText) {
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%I:%M", &localTime_s); // Time part

    text.setString(buffer);

    // Set AM/PM
    std::string amPm = (localTime_s.tm_hour >= 12) ? "PM" : "AM";
    amPmText.setString(amPm);

    // Adjust AM/PM position based on the digital clock's width
    float offsetX = text.getGlobalBounds().width + 10; // 10px spacing after the time part
    amPmText.setPosition(text.getPosition().x + offsetX, text.getPosition().y + 5); // +5 for slight adjustment
}

// Function to update the analog clock
void updateAnalogClock(sf::RectangleShape& hourHand, sf::RectangleShape& minuteHand, sf::RectangleShape& secondHand) {
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    // Calculate the angles based on the current time
    float hourAngle = (localTime_s.tm_hour % 12) * 30.f; // 360 / 12 = 30 degrees per hour
    float minuteAngle = localTime_s.tm_min * 6.f; // 360 / 60 = 6 degrees per minute
    float secondAngle = localTime_s.tm_sec * 6.f; // 360 / 60 = 6 degrees per second

    // Set the rotation for each hand (clockwise)
    hourHand.setRotation(90 + (hourAngle + (localTime_s.tm_min / 2.0f))); // Adjust for minutes in hour
    minuteHand.setRotation(90 + minuteAngle); // Correct minute hand rotation
    secondHand.setRotation(secondAngle);  // Correct second hand rotation
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Clock UI");
    window.setFramerateLimit(60);

    // Ask the user for a time zone and get the offset
    int timeZoneOffset = selectTimeZone();  // Get the time zone offset based on user input

    // Ask user if they want to enable dynamic background
    cout << "Would you like to enable dynamic background? (yes/no): ";
    string userInput;
    cin >> userInput;

    bool enableDynamicBackground = (userInput == "yes");


    // Firework Manager
    FireworkManager fireworkManager(window);
    bool showFireworks = false; // Fireworks toggle

    // Plain black background
    auto background = createPlainBackground(window.getSize());

    // Font setup
    sf::Font font;
    if (!font.loadFromFile("C:/Users/Zone Tech/Desktop/hojaosimsim/project/assets/DSEG7Classic-Regular.ttf")) {
        return -1;
    }

    // Digital clock
    sf::Text digitalClock;
    digitalClock.setFont(font);
    digitalClock.setCharacterSize(68); // Smaller size
    digitalClock.setFillColor(sf::Color::White);
    digitalClock.setPosition(90, 220); // Moved higher

    // AM/PM label
    sf::Text amPmText;
    amPmText.setFont(font);
    amPmText.setCharacterSize(20); // Smaller size
    amPmText.setFillColor(sf::Color::White);
    amPmText.setPosition(digitalClock.getPosition().x + digitalClock.getGlobalBounds().width + 14, 224); // Positioned right next to time

    // Analog clock
    sf::CircleShape clockFace(100);
    clockFace.setFillColor(sf::Color(255, 255, 255));
    clockFace.setOutlineColor(sf::Color(0, 0, 139)); // Dark blue outline (RGB values for Dark Blue)
    clockFace.setOutlineThickness(2.5); // Thin border 
    clockFace.setOrigin(100, 100);
    clockFace.setPosition(550, 220); // Moved higher

    sf::RectangleShape hourHand(sf::Vector2f(50, 6));
    hourHand.setFillColor(sf::Color::Black);
    hourHand.setOrigin(50, 3);  // Origin to the center of the clock
    hourHand.setPosition(550, 220);  // Center of clock face

    sf::RectangleShape minuteHand(sf::Vector2f(70, 4));
    minuteHand.setFillColor(sf::Color::Black);
    minuteHand.setOrigin(70, 2);  // Origin to the center of the clock
    minuteHand.setPosition(550, 220);  // Center of clock face

    sf::RectangleShape secondHand(sf::Vector2f(90, 2));
    secondHand.setFillColor(sf::Color::Red);
    secondHand.setOrigin(90, 1);  // Origin to the center of the clock
    secondHand.setPosition(550, 220);  // Center of clock face

    // Square Buttons
    sf::RectangleShape buttons[3];
    sf::Text buttonText[3];

    std::string labels[3] = { "R", "F", "G" };

    for (int i = 0; i < 3; ++i) {
        // Button appearance (square)
        buttons[i].setSize(sf::Vector2f(60, 60)); // Square size
        buttons[i].setFillColor(sf::Color(0, 0, 139)); // Dark blue
        buttons[i].setOrigin(30, 30);
        buttons[i].setPosition(300 + i * 100, 450);

        // Button text
        buttonText[i].setFont(font);
        buttonText[i].setString(labels[i]);
        buttonText[i].setCharacterSize(15); // Adjust text size
        buttonText[i].setFillColor(sf::Color::White);
        sf::FloatRect textBounds = buttonText[i].getLocalBounds();
        buttonText[i].setOrigin(textBounds.width / 2, textBounds.height / 2);
        buttonText[i].setPosition(buttons[i].getPosition().x, buttons[i].getPosition().y + 5);
    }

    // Time-based color logic
    std::time_t currentTime = std::time(nullptr);
    std::tm localTime_s;
    localtime_s(&localTime_s, &currentTime);

    // Default colors for night
    sf::Color backgroundColor = sf::Color::Black;
    sf::Color buttonColor = sf::Color(0, 0, 139);  // Dark Blue
    sf::Color outlineColor = sf::Color(0, 0, 139); // Dark Blue

    // Morning (6 AM - 12 PM)
    if (localTime_s.tm_hour >= 6 && localTime_s.tm_hour < 12) {
        backgroundColor = sf::Color(173, 216, 230);  // Light Blue
        buttonColor = sf::Color(255, 255, 102);      // Light Yellow
        outlineColor = sf::Color(255, 255, 102);     // Light Yellow
    }
    // Evening (12 PM - 6 PM)
    else if (localTime_s.tm_hour >= 12 && localTime_s.tm_hour < 18) {
        backgroundColor = sf::Color(139, 69, 19);    // Brown
        buttonColor = sf::Color(0, 100, 0);          // Dark Green
        outlineColor = sf::Color(0, 100, 0);         // Dark Green
    }

    if (enableDynamicBackground) {
        // Update colors if dynamic background is enabled
        background[0].color = backgroundColor;
        background[1].color = backgroundColor;
        background[2].color = backgroundColor;
        background[3].color = backgroundColor;

        // Update button and clock face outline colors
        for (int i = 0; i < 3; ++i) {
            buttons[i].setFillColor(buttonColor);
        }
        clockFace.setOutlineColor(outlineColor);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 3; ++i) {
                    if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        cout << labels[i] << " Button clicked!" << endl;

                        if (labels[i] == "R") {
                            addReminder();  // Add reminder functionality when "R" is clicked
                        }
                        if (labels[i] == "F") {
                            showFireworks = true;  // Enable fireworks
                        }
                        if (labels[i] == "G") {
                            //cout<< "Which game would you like to play:/n1.Falling Stars/n2.Arkanoid"
                            //cin<< chosen_G;
                            //if (chosen_G == 1){
                                //execute/switch to Faliing stars game file/(window);
                            //}else{
                                ////execute/switch to Faliing stars game file/(window);
                            //}

                        }
                    }
                }
            }
        }



        // Fetch the current time based on the selected time zone
        std::tm localTime_s = getTimeForTimeZone(timeZoneOffset);

        // Update the dynamic background based on the hour
        if (enableDynamicBackground) {
            if (localTime_s.tm_hour >= 6 && localTime_s.tm_hour < 12) {
                backgroundColor = sf::Color(173, 216, 230);  // Light Blue
                buttonColor = sf::Color(255, 255, 102);      // Light Yellow
                outlineColor = sf::Color(255, 255, 102);     // Light Yellow
            }
            else if (localTime_s.tm_hour >= 12 && localTime_s.tm_hour < 18) {
                backgroundColor = sf::Color(139, 69, 19);    // Brown
                buttonColor = sf::Color(0, 100, 0);          // Dark Green
                outlineColor = sf::Color(0, 100, 0);         // Dark Green
            }
            else {
                backgroundColor = sf::Color::Black;          // Default night colors
                buttonColor = sf::Color(0, 0, 139);          // Dark Blue
                outlineColor = sf::Color(0, 0, 139);         // Dark Blue
            }

            // Apply the background color
            background[0].color = backgroundColor;
            background[1].color = backgroundColor;
            background[2].color = backgroundColor;
            background[3].color = backgroundColor;

            // Update button and clock face outline colors
            for (int i = 0; i < 3; ++i) {
                buttons[i].setFillColor(buttonColor);
            }
            clockFace.setOutlineColor(outlineColor);
        }

        // Update digital and analog clocks
        updateDigitalClock(digitalClock, amPmText, timeZoneOffset);
        updateAnalogClock(hourHand, minuteHand, secondHand, timeZoneOffset);

        // Check reminders every frame
        checkReminders();  // Check and display any reminders that should be triggered

        // Render everything
        window.clear();
        window.draw(background); // Draw the updated background
        window.draw(digitalClock);
        window.draw(amPmText);
        window.draw(clockFace);
        window.draw(hourHand);
        window.draw(minuteHand);
        window.draw(secondHand);
        for (int i = 0; i < 3; ++i) {
            window.draw(buttons[i]);
            window.draw(buttonText[i]);
        }
        // Draw fireworks if enabled
        if (showFireworks) {
            fireworkManager.update();
        }

        window.display();
    }

    return 0;
}*/