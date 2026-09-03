#pragma once
#include <SFML/Graphics.hpp>
class Player{
    private:
    sf::RectangleShape bodyP;
    bool inAir;
    float speed;


    public:
    Player();
    const sf::RectangleShape& getPlayerBody()const;
    void checkJump();
    void Jump();

    void reset();

};