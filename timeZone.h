#ifndef TIMEZONE_H
#define TIMEZONE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

// Declare the map for time zone offsets
extern std::map<std::string, int> timeZoneOffsets;

// Function declarations
std::tm getTimeForTimeZone(int offset);
void updateDigitalClock(sf::Text& text, sf::Text& amPmText, int offset);
void updateAnalogClock(sf::RectangleShape& hourHand, sf::RectangleShape& minuteHand, sf::RectangleShape& secondHand, int offset);
int selectTimeZone();

#endif // TIMEZONE_H
#pragma once
