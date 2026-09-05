#include<SFML/Graphics.hpp>
#include"powerup.hpp"

Powerup::Powerup(sf::Vector2f givenLocation, float givenSpeed)
    :power(25.f),
    speed(givenSpeed),
    respawn(false),
    ifPowerAdded(false),
    outOfBounds(false)
{
    power.setOrigin({25.f,25.f});
    power.setPosition(givenLocation);
    power.setFillColor(sf::Color::Green);
}

const sf::CircleShape& Powerup::getPowerBody()const{
    return power;
}

sf::CircleShape& Powerup::getPowerBody(){
    return power;
}

float Powerup::getPowerSpeed(){
    return speed;
}

void Powerup::setPowerSpeed(float dspeed){
    speed = dspeed;
}

void Powerup::move(){
    if(!outOfBounds)
        power.move({speed,0.f});
    if(power.getPosition().x < -(power.getRadius())){
        outOfBounds = true;
        respawn = true;
    }
}

void Powerup::setRespawn(int a){
    if(a == 0){
        respawn = false;
    }
    else{
        respawn = true;
    }
}

const bool Powerup::getPowerAddStatus() const{
    return ifPowerAdded;
}

void Powerup::setPowerAddStatus(int a) {
    if(a == 0){
        ifPowerAdded = false;
    }
    else{
        ifPowerAdded = true;
    }
}

const bool Powerup::getIfOutOfBounds() const{
    return outOfBounds;
}

void Powerup::setBounds(int a){
    if(a == 0){
        outOfBounds = false;
    }
    else{
        outOfBounds = true;
    }
}
