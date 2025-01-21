#pragma once
#include "Particle.h"
#include <vector>
#include <memory>

#define BANG_SIZE 60

class Firework
{
    float secondsToFade;
    std::shared_ptr<ParticleBase> rocket;
    std::vector<ParticleExplosion> bang;
    bool vanished;
    bool bangVanished;

public:
    Firework(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);
    Firework(const Firework& fwork);
    Firework& operator=(const Firework& fwork);
    ~Firework();

    // Add the isFinished() declaration here
    bool isFinished() const;
};
