#include <SFML/Graphics.hpp>
#include "player.hpp"
Player::Player()
        :bodyP({80,50}),
        inAir(false),
        speed(0.f),
        hp(4),
        isBurning(false)
{
    bodyP.setPosition({80,345});
    bodyP.setFillColor(sf::Color(56, 189, 248));
}

const sf::RectangleShape& Player::getPlayerBody()const{
    return bodyP;
}


void Player::setPlacement(sf::Vector2f coordinates){
    bodyP.setPosition(coordinates);
}

void Player::checkJump(){
    if(bodyP.getPosition().y + bodyP.getSize().y == 395 && (!inAir)){
        speed = -21.5;
        inAir = true;
    }
}

void Player::reset(){
    bodyP.setPosition({80,345});
    hp = 4;
    inAir = false;
    speed = 0.f;
    isBurning = false;

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

const int Player::getPlayerhp() const{
    return hp;
}

void Player::changeHP(int damage){
    hp = hp - damage;
    if(hp<=0){
        hp = 0;
    }
    if(hp>=4){
        hp = 4;
    }
}

void Player::setBurningTrue(){
   isBurning = true;
}

void Player::setBurningFalse(){
   isBurning = false;
}

const bool Player::getBurningState() const{
    return isBurning;
}

const bool Player::isPlayerJumping() const {
    return inAir;
}

void Player::changeWorld(){
    if(bodyP.getPosition().y <= 395){
        bodyP.setPosition({80,405});
    }
    else{
        bodyP.setPosition({80,345});
    }
}
