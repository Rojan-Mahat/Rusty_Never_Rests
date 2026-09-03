#pragma once
#include <SFML/Graphics.hpp>
class Player{
    private:
    sf::RectangleShape bodyP;
    bool inAir;
    int hp;
    float speed;


    public:
    Player();
    const sf::RectangleShape& getPlayerBody()const;
    void checkJump();
    void Jump();
    const int getPlayerhp() const;
    void changeHP(int damage);

    void reset();

};