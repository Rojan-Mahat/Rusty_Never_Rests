#pragma once
#include "player.hpp"
#include "enemy.hpp"
#include "fireball.hpp"
#include"powerup.hpp"
#include<SFML/Graphics.hpp>
#include<random>
class Game{
    private:
    sf::RenderWindow screen;
    sf::Clock frameClock;
    Player player;
    std::vector<Enemy> enemies;
    bool paused;
    int score;
    int bestScore;
    sf::RectangleShape runningPath;
    sf::Font scoreFont;
    sf::Text scoreText;
    sf::Text hpText;
    sf::Text oxygenText;
    sf::Text bestScoreText;
    sf::Text powerDurationText;
    Fireball fire;
    bool onLand;
    float oxygen;
    float drownTimer;
    float burnTimer;
    float electricityTime;
    float gameTimer;
    Powerup power;
    float powerUpTimer;
    bool collisionShield;
    bool electricalShield;
    bool poweredState;
    bool flightActive;
    bool godActive;
    std::vector<float> initial_speed_enemy;
    int summonGod;
    float powerDuration;

    float oxygenBlinkTimer = 0.f;
    bool oxygenTextVisible = true;

    bool electricWarning = false;
    bool warningRed = true;
    float warningFlashTimer = 0.f;
    


    




    public:
    Game();
    sf::RectangleShape createGround();
    void drawGround();
    void drawPlayerBody();
    void drawEnemyBody();
    void drawFireballBody();
    void drawPowerUps();
    void checkCollision();
    void checkFireContact();
    void run();
    void checkPlayer(float dt);
    float createRandom(int a, int b);
    

    void runPowerUps();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void reset();
    void stateChanger();
    void trackOxygen(float dt);
    void damageByBurning(float dt);
    void PowerCollision();
    void reStartPowerUps();
    void providePower();

    void updateScore();
    void drawTexts();
    void updateOxygenText(float dt);

    void electricution(float dt);
    float electricityTimeGenerator();
    void checkElectricution();
    void drawElectricWarning();

    void noDamage();
    void flightMode();
    void resistElectricity();
    void godMode();

    void enable_collision_shield();
    void disable_collision_shield();
    void enable_electrical_shield();
    void disable_electrical_shield();
    void setRandomType();
    
};