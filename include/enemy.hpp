#pragma once
#include<SFML/Graphics.hpp>
class Enemy{
    private:
    sf::RectangleShape bodyE;
    float speed;
    bool respawned;
    int damage;
    bool didIscore;



    public:
    Enemy(sf::Vector2f , float speed);
    const sf::RectangleShape& getEnemyBody() const;
    void move();
    void reset();

    bool getRespawnedStatus();
    void increaseDifficulty(float mag);
    float createRandomSpawn();
    const int getEnemyDamage()const;
    const bool getPassStatus(float x_coordinate) const;
    bool getScoreStatus();
    void markScored();
    void unmarkScored();

};