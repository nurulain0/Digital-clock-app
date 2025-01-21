#include "timeZone.h"
#include <iostream>
#include <ctime>

// Define the map for time zone offsets
std::map<std::string, int> timeZoneOffsets = {
    {"UTC", 0},//
    {"GMT", 0},//
    {"CST", -6},//
    {"EST", -5},//
    {"PST", -8},//
    {"IST", 5},//
    {"JST", 9},//Japan Std time
    {"PKT", 5}, // Pakistan Standard Time
};

// Corrected getTimeForTimeZone function
std::tm getTimeForTimeZone(int offset) {
    // Get the current UTC time in seconds since the epoch
    std::time_t currentTime = std::time(nullptr);

    // Adjust the time by the offset in seconds
    currentTime += offset * 3600;

    // Convert the adjusted time to a local `std::tm` structure
    std::tm adjustedTime;
    gmtime_s(&adjustedTime, &currentTime); // Use gmtime_s for UTC adjustment

    return adjustedTime;
}


void updateDigitalClock(sf::Text& text, sf::Text& amPmText, int offset) {
    std::tm localTime_s = getTimeForTimeZone(offset);

    char buffer[16];
    std::strftime(buffer, sizeof(buffer), "%I:%M", &localTime_s);
    text.setString(buffer);

    // Set AM/PM
    std::string amPm = (localTime_s.tm_hour >= 12) ? "PM" : "AM";
    amPmText.setString(amPm);

    // Adjust AM/PM position based on the digital clock's width
    float offsetX = text.getGlobalBounds().width + 10;
    amPmText.setPosition(text.getPosition().x + offsetX, text.getPosition().y);
}

void updateAnalogClock(sf::RectangleShape& hourHand, sf::RectangleShape& minuteHand, sf::RectangleShape& secondHand, int offset) {
    std::tm localTime_s = getTimeForTimeZone(offset);

    // Calculate the angles based on the adjusted time
    float hourAngle = (localTime_s.tm_hour % 12) * 30.f;
    float minuteAngle = localTime_s.tm_min * 6.f;
    float secondAngle = localTime_s.tm_sec * 6.f;

    // Set the rotation for each hand (clockwise)
    hourHand.setRotation(90 + (hourAngle + (localTime_s.tm_min / 2.0f)));
    minuteHand.setRotation(90 + minuteAngle);
    secondHand.setRotation(secondAngle);
}

int selectTimeZone() {
    std::cout << "Please select a time zone:" << std::endl;
    for (const auto& zone : timeZoneOffsets) {
        std::cout << zone.first << " (" << zone.second << " UTC)" << std::endl;
    }

    std::string choice;
    std::cout << "Enter the time zone (e.g., 'CST', 'EST'): ";
    std::cin >> choice;

    if (timeZoneOffsets.find(choice) != timeZoneOffsets.end()) {
        return timeZoneOffsets[choice];
    }
    else {
        std::cout << "Invalid time zone. Defaulting to UTC." << std::endl;
        return 0;
    }
}
