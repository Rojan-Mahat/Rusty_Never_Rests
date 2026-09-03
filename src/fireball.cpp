#include<SFML/Graphics.hpp>
#include "fireball.hpp"
#include <random>

Fireball::Fireball()
    :fire(25.f),
    speed(-8.5f),
    respawned(true),
    damage(1)

{
    fire.setOrigin({25.f,25.f});
    fire.setPosition(createRandomSpawn());
    fire.setFillColor(sf::Color::Yellow);
}


sf::Vector2f Fireball::createRandomSpawn(){
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_real_distribution<float> distX(2500, 5000);
    std::uniform_real_distribution<float> distY(85.f, 370.f);

    float x = distX(gen);
    float y = distY(gen);
    
    return {x,y};
}

void Fireball::reset(){
    fire.setPosition(createRandomSpawn());
    speed = -8.5f;
}

const sf::CircleShape& Fireball::getFireballBody() const{
    return fire;
}

void Fireball::move(){
    fire.move({speed,0.f});
    respawned = false;
    if(fire.getPosition().x <= -(fire.getRadius())){
        fire.setPosition(createRandomSpawn());
        respawned = true;    
    }
}

bool Fireball::getRespawnedStatus(){
    return respawned;
}

const int Fireball::getFireDamage()const{
    return damage;
}