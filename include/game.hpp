#pragma once
#include "player.hpp"
#include "enemy.hpp"
#include "fireball.hpp"
#include<SFML/Graphics.hpp>
#include<random>
class Game{
    private:
    sf::RenderWindow screen;
    sf::Clock frameClock;
    Player player;
    std::vector<Enemy> enemies;
    bool collision;
    bool paused;
    int score;
    int bestScore;
    sf::RectangleShape runningPath;
    sf::Font scoreFont;
    sf::Text scoreText;
    sf::Text hpText;
    sf::Text oxygenText;
    sf::Text bestScoreText;
    Fireball fire;
    bool onLand;
    float oxygen;
    float drownTimer;
    float burnTimer;
    float electricityTime;
    float gameTimer;

    float oxygenBlinkTimer = 0.f;
    bool oxygenTextVisible = true;

    bool electricWarning = false;
    bool warningRed = true;
    float warningFlashTimer = 0.f;


    




    public:
    Game();
    sf::RectangleShape createGround();
    sf::RectangleShape createScoreBoard();
    void drawGround();
    void drawPlayerBody();
    void drawEnemyBody();
    void drawFireballBody();
    void checkCollision();
    void checkFireContact();
    void run();
    void checkPlayer(float dt);

    void pauseGame();
    void resumeGame();
    void restartGame();
    void reset();
    void stateChanger();
    void trackOxygen(float dt);
    void damageByBurning(float dt);

    void updateScore();
    void drawTexts();
    void updateOxygenText(float dt);

    void electricution(float dt);
    float electricityTimeGenerator();
    void checkElectricution();
    void drawElectricWarning();
};