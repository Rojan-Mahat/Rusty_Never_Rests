#pragma once
#include<SFML/Graphics.hpp>

class Fireball{
    private:
    sf::CircleShape fire;
    float speed;
    bool respawned;




    public:
    Fireball();
    void reset();
    const sf::CircleShape& getFireballBody() const;
    void move();
    sf::Vector2f createRandomSpawn();
    bool getRespawnedStatus();
    void setSpeed(float Speed);
};