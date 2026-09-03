#pragma once
#include<SFML/Graphics.hpp>
class Enemy{
    private:
    sf::RectangleShape bodyE;
    float speed;
    bool respawned;



    public:
    Enemy();
    const sf::RectangleShape& getEnemyBody() const;
    void move();
    void reset();

    bool getRespawnedStatus();
    void increaseDifficulty(float mag);
    float createRandomSpawn();

};