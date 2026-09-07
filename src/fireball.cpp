#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "fireball.hpp"
#include "world.hpp"
#include <random>

Fireball::Fireball()
    :fire(25.f),
    speed(-20.f),
    respawned(true),
    fireSpawnBuffer("assets/audio/fireSpawn.mp3"),
    fireSpawnSound(fireSpawnBuffer),

    fireTexture("assets/sprites/enemies/fireball.png"),
    fireSprite(fireTexture)
{
    fire.setOrigin({25.f,25.f});
    fire.setPosition(createRandomSpawn());
    fireSprite.setScale({0.08f, 0.08f});
    fire.setFillColor(sf::Color(255,0,0,100));

    fireSprite.setPosition({fire.getPosition().x - 45.f , fire.getPosition().y - 45.f});
}


sf::Vector2f Fireball::createRandomSpawn(){
    std::random_device rd;
    std::mt19937 gen{rd()};

    std::uniform_real_distribution<float> distX(4000,9000);
    std::uniform_real_distribution<float> distY(85.f, World::FIRE_MAX_Y);

    float x = distX(gen);
    float y = distY(gen);
    
    return {x,y};
}

void Fireball::reset(){
    fire.setPosition(createRandomSpawn());
    fireSpawnSound.play();
    speed = -20.f;
    fireSprite.setPosition({fire.getPosition().x - 45.f , fire.getPosition().y - 45.f});
}

const sf::CircleShape& Fireball::getFireballBody() const{
    return fire;
}

void Fireball::move(){
    fire.move({speed,0.f});
    respawned = false;
    if(fire.getPosition().x <= -(fire.getRadius())){
        fire.setPosition(createRandomSpawn());
        fireSpawnSound.play();
        respawned = true;    
    }
    fireSprite.setPosition({fire.getPosition().x - 37.f , fire.getPosition().y - 45.f});
}

bool Fireball::getRespawnedStatus(){
    return respawned;
}

void Fireball::setSpeed(float Speed){
    speed = Speed;
}

const sf::Sprite& Fireball::getFireballSprite() const{
    return fireSprite;
}