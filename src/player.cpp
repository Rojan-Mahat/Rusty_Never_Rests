#include <SFML/Graphics.hpp>
#include "player.hpp"
Player::Player()
        :bodyP({80,50}),
        inAir(false),
        speed(0.f)
{
    bodyP.setPosition({80,345});
    bodyP.setFillColor(sf::Color::Blue);
}

const sf::RectangleShape& Player::getPlayerBody()const{
    return bodyP;
}

void Player::checkJump(){
    if(bodyP.getPosition().y + bodyP.getSize().y == 395 && (!inAir)){
        speed = -20.0;
        inAir = true;
    }
}

void Player::Jump(){
    if(inAir){
        bodyP.move({0.f,speed});
        speed++;
    }
    if(bodyP.getPosition().y + bodyP.getSize().y == 395){
        inAir = false;
        speed = 0.f;
        return;
    }
}
