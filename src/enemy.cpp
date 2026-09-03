#include "enemy.hpp"
#include<SFML/Graphics.hpp>
#include<random>

Enemy::Enemy()
      :bodyE({40.f,100.f}),
      speed(-6.5f),
      respawned(true),
      damage(4)
{
    bodyE.setPosition({1160.f,295.f});
    bodyE.setFillColor(sf::Color(251, 113, 133));
}

void Enemy::move(){
    bodyE.move({speed,0.f});
    respawned = false;
    if(bodyE.getPosition().x <= -(bodyE.getSize().x)){
        bodyE.setPosition({createRandomSpawn(),295.f});
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
    bodyE.setPosition({1160.f,295.f});
    speed = -6.5f;
}

const sf::RectangleShape& Enemy::getEnemyBody() const{
    return bodyE;
}

const int Enemy::getEnemyDamage()const{
    return damage;
}