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
    int score;
    int bestScore;
    sf::RectangleShape runningPath;
    bool scoreAwarded;
    sf::Font scoreFont;
    sf::Text scoreText;
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
    void run();

    void pauseGame();
    void resumeGame();
    void restartGame();
    void reset();

    void updateScore();
    void drawScore();
};