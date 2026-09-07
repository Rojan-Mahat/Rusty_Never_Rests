#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
class Fireball{
    private:
    sf::CircleShape fire;
    float speed;
    bool respawned;
    sf::SoundBuffer fireSpawnBuffer;
    sf::Sound fireSpawnSound;

    sf::Texture fireTexture;
    sf::Sprite fireSprite;




    public:
    Fireball();
    void reset();
    const sf::CircleShape& getFireballBody() const;
    void move();
    sf::Vector2f createRandomSpawn();
    bool getRespawnedStatus();
    void setSpeed(float Speed);

    const sf::Sprite& getFireballSprite() const;

};