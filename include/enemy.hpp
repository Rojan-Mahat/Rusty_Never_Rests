#pragma once
#include<SFML/Graphics.hpp>
class Enemy{
    private:
    sf::RectangleShape bodyE;
    float speed;
    bool respawned;
    int damage;
    bool didIscore;
    bool didIdamage;



    public:
    Enemy(sf::Vector2f , float speed);
    const sf::RectangleShape& getEnemyBody() const;
    void move();
    void reset();
    
    float getSpeed() const;
    void setSpeed(float Speed);
    bool getRespawnedStatus();
    void increaseDifficulty(float mag);
    float createRandomSpawn();
    const int getEnemyDamage()const;
    const bool getPassStatus(float x_coordinate) const;
    bool getScoreStatus();
    void markScored();
    void unmarkScored();
    void markDamage();
    void unmarkDamage();
    bool getDamageStatus();

};