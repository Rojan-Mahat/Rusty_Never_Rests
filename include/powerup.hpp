#pragma once
#include<SFML/Graphics.hpp>

class Powerup{
    private:
    sf::CircleShape power;
    float speed;
    bool respawn;
    bool ifPowerAdded;
    bool outOfBounds;






    public:
    Powerup(sf::Vector2f givenLocation, float givenSpeed);
    const sf::CircleShape& getPowerBody()const;
    sf::CircleShape& getPowerBody();
    float getPowerSpeed();
    void setPowerSpeed(float dspeed);
    void move();
    const bool getRespawnStatus() const;
    void setRespawn(int a);
    const bool getPowerAddStatus() const;
    void setPowerAddStatus(int a);
    const bool getIfOutOfBounds() const;
    void setBounds(int a);

    
};
