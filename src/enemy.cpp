#include "enemy.hpp"
#include<SFML/Graphics.hpp>
#include<random>
#include<cmath>
#include "world.hpp"

Enemy::Enemy(sf::Vector2f givenPosition , float givenSpeed , const sf::Texture& texture, sf::Vector2f spriteOffset, sf::Vector2f hitboxSize,float givenHitboxOffsetX)
      :bodyE(hitboxSize),
      speed(givenSpeed),
      respawned(false),
      damage(2),
      didIscore(false),
      didIdamage(false),
      enemySprite(texture),
      spriteOffsetX(spriteOffset.x),
      spriteOffsetY(spriteOffset.y),
      hitboxOffsetX(givenHitboxOffsetX)


{
    bodyE.setPosition({givenPosition.x + hitboxOffsetX,World::GROUND_Y - bodyE.getSize().y});
    bodyE.setFillColor(sf::Color(251, 113, 133));

    enemySprite.setScale({0.10f, 0.10f});
    enemySprite.setPosition({bodyE.getPosition().x - hitboxOffsetX + spriteOffsetX, World::ENEMY_Y + spriteOffsetY});
  
}

void Enemy::move(float otherEnemy){
    bodyE.move({speed,0.f});
    respawned = false;
    if(bodyE.getPosition().x <= -(bodyE.getSize().x)){
        float newX = createRandomSpawn();   
        if(std::abs(otherEnemy - newX) < 450.f)
            newX = otherEnemy + 550.f;
        bodyE.setPosition({newX + hitboxOffsetX ,World::GROUND_Y - bodyE.getSize().y});
        didIdamage = false;
        didIscore = false;
        respawned = true;    
    }
    enemySprite.setPosition({bodyE.getPosition().x - hitboxOffsetX + spriteOffsetX, World::ENEMY_Y + spriteOffsetY});
}

float Enemy::createRandomSpawn(){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> dist(1150, 2000);
    float extra = dist(gen);
    return extra;
}

void Enemy::increaseDifficulty(float mag){
   if(speed<=-13)
        speed -= mag;
    
}

bool Enemy::getRespawnedStatus(){
    return respawned;
}

void Enemy::reset(){
    float newX = createRandomSpawn();
    bodyE.setPosition({newX + hitboxOffsetX,World::GROUND_Y - bodyE.getSize().y});
    enemySprite.setPosition({bodyE.getPosition().x - hitboxOffsetX + spriteOffsetX, World::ENEMY_Y + spriteOffsetY});
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

const sf::Sprite& Enemy::getEnemySprite() const{
    return enemySprite;
}