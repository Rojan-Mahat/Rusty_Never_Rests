#pragma once
#include <SFML/Graphics.hpp>
class Player{
    private:
    sf::RectangleShape bodyP;
    bool inAir;
    int hp;
    float speed;
    bool isBurning;


    public:
    Player();
    const sf::RectangleShape& getPlayerBody()const;
    void checkJump();
    void Jump();
    const int getPlayerhp() const;
    void changeHP(int damage);
    void changeHP(float damage);

    void reset();
    const bool isPlayerJumping() const;
    void changeWorld();
    void setBurningTrue();
    void setBurningFalse();
    const bool getBurningState() const;

};