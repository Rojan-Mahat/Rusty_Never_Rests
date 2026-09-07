#include<SFML/Graphics.hpp>
#include"powerup.hpp"

Powerup::Powerup(sf::Vector2f givenLocation, float givenSpeed, Type whatTypeofPower)
    :power(25.f),
    speed(givenSpeed),
    type(whatTypeofPower),
    respawn(false),
    ifPowerAdded(false),
    outOfBounds(false),
    powerTexture("assets/sprites/powerups/powerup.png"),
    powerSprite(powerTexture)
{
    power.setFillColor(sf::Color(255,0,0,100));
    power.setOrigin({25.f,25.f});
    power.setPosition(givenLocation);

    powerSprite.setScale({0.12f, 0.12f});
    setSprite();

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
    powerSprite.setPosition(power.getPosition());
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

Powerup::Type Powerup::getType(){
    return type;
}

void Powerup::setType(Type newType){
    type = newType;
    setSprite();
}

const sf::Sprite& Powerup::getPowerSprite() const{
    return powerSprite;
}

void Powerup::setSprite(){
    if(type == Type::Health)
        powerSprite.setTextureRect(sf::IntRect({0,0},{512,512}));

    else if(type == Type::Shield)
        powerSprite.setTextureRect(sf::IntRect({512,0},{512,512}));
    

    else if(type == Type::Flight)
        powerSprite.setTextureRect(sf::IntRect({1024,0},{512,512}));
    

    else if(type == Type::ElectricResist)
        powerSprite.setTextureRect(sf::IntRect({0,512},{512,512}));
    

    else if(type == Type::GodMode)
        powerSprite.setTextureRect(sf::IntRect({512,512},{512,512}));

    powerSprite.setOrigin({256.f,256.f});
    powerSprite.setPosition(power.getPosition());
}