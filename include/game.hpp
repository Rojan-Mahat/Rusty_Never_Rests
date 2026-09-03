#pragma once
#include "player.hpp"
#include "enemy.hpp"
#include "fireball.hpp"
#include<SFML/Graphics.hpp>
class Game{

    private:
    sf::RenderWindow screen;
    Player player;
    Enemy enemy;
    bool collision;
    bool paused;
    bool burn;
    int score;
    int bestScore;
    sf::RectangleShape runningPath;
    bool scoreAwarded;
    sf::Font scoreFont;
    sf::Text scoreText;
    sf::Text hpText;
    sf::Text bestScoreText;
    Fireball fire;


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
    void checkPlayer();

    void pauseGame();
    void resumeGame();
    void restartGame();
    void reset();

    void updateScore();
    void drawTexts();
};