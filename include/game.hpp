#pragma once
#include "player.hpp"
#include "enemy.hpp"
#include<SFML/Graphics.hpp>
class Game{
    private:
    sf::RenderWindow screen;
    Player player;
    Enemy enemy;
    sf::RectangleShape runningPath;

    public:
    Game();
    sf::RectangleShape createGround();
    void drawGround();
    void drawPlayerBody();
    void run();

    
};