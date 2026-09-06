#include "enemy.hpp"
#include<SFML/Graphics.hpp>
#include<random>
#include<cmath>

Enemy::Enemy(sf::Vector2f givenPosition , float givenSpeed)
      :bodyE({40.f,100.f}),
      speed(givenSpeed),
      respawned(false),
      damage(2),
      didIscore(false),
      didIdamage(false)

{
    bodyE.setPosition(givenPosition);
    bodyE.setFillColor(sf::Color(251, 113, 133));
}

void Enemy::move(float otherEnemy){
    bodyE.move({speed,0.f});
    respawned = false;
    if(bodyE.getPosition().x <= -(bodyE.getSize().x)){
        float newX = createRandomSpawn();   
        if(std::abs(otherEnemy - newX) < 150.f)
            newX = otherEnemy + 200.f;
        bodyE.setPosition({newX,295.f});
        didIdamage = false;
        didIscore = false;
        respawned = true;    
    }
}

float Enemy::createRandomSpawn(){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> dist(1150, 2000);
    float extra = dist(gen);
    return extra;
}

void Enemy::increaseDifficulty(float mag){
    speed -= mag;
}

bool Enemy::getRespawnedStatus(){
    return respawned;
}

void Enemy::reset(){
    bodyE.setPosition({createRandomSpawn(),295.f});
    speed = -6.5f;
    didIscore = false;
    didIdamage = false;
}

const sf::RectangleShape& Enemy::getEnemyBody() const{
    return bodyE;
}

const int Enemy::getEnemyDamage()const{
    return damage;
}

void Enemy::markScored(){
    didIscore = true;
}

void Enemy::unmarkScored(){
    didIscore = false;
}

void Enemy::markDamage(){
    didIdamage = true;
}

void Enemy::unmarkDamage(){
    didIdamage = false;
}

bool Enemy::getDamageStatus() {
    return didIdamage;
}

const bool Enemy::getPassStatus(float x_coordinate) const{
    return (bodyE.getPosition().x <= x_coordinate);
}

bool Enemy::getScoreStatus() {
    return didIscore;
}

float Enemy::getSpeed()const{
    return speed;
}
void Enemy::setSpeed(float Speed){
    speed = Speed;
}