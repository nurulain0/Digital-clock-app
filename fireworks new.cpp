#include "Firework.h"

Firework::Firework(sf::RenderWindow& window)
{
    rocket = std::make_shared<ParticleBase>(window);
    vanished = false;
    bangVanished = false;
    secondsToFade = 1.f;
}

void Firework::update(sf::RenderWindow& window)
{
    if (rocket != nullptr)
    {
        rocket->update();
        rocket->render(window);
        if (rocket->falling())
        {
            for (int i = 0; i < BANG_SIZE; ++i)
            {
                bang.push_back(ParticleExplosion(rocket->getPosition()));
            }
            rocket = nullptr;
        }
    }

    bool notEverythingFaded = false;
    for (auto& x : bang)
    {
        if (!x.exploded())
        {
            x.update();
            x.render(window);
            notEverythingFaded = true;
        }
    }

    if (!notEverythingFaded && rocket == nullptr)
    {
        if (rand() % 101 < 10)
            *this = Firework(window);
    }
}

Firework::Firework(const Firework& fwork)
{
    rocket = fwork.rocket;
    bang = fwork.bang;
}

Firework& Firework::operator=(const Firework& fwork)
{
    if (this == &fwork)
        return *this;

    rocket = fwork.rocket;
    bang = fwork.bang;
    return *this;
}

Firework::~Firework()
{
}

// Add the isFinished() method here
bool Firework::isFinished() const
{
    if (rocket != nullptr) {
        return false; // Rocket is still active
    }

    for (const auto& particle : bang) {
        if (!particle.exploded()) {
            return false; // Some particles are still visible
        }
    }

    return true; // Rocket is gone, and all particles have faded
}
