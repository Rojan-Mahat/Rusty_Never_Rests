#pragma once
#include<SFML/Graphics.hpp>

class Powerup{
    public:
    enum class Type{
        Health,
        Shield,
        Flight,
        ElectricResist,
        GodMode
    };


    private:
    sf::CircleShape power;
    float speed;
    bool respawn;
    bool ifPowerAdded;
    bool outOfBounds;
    Type type;


    public:
    Powerup(sf::Vector2f givenLocation, float givenSpeed,Type whatTypeofPower);
    const sf::CircleShape& getPowerBody()const;
    sf::CircleShape& getPowerBody();
    float getPowerSpeed();
    void setPowerSpeed(float dspeed);
    void move();
    void setRespawn(int a);
    const bool getPowerAddStatus() const;
    void setPowerAddStatus(int a);
    const bool getIfOutOfBounds() const;
    void setBounds(int a);
    Type getType();
    void setType(Type newType);

};
