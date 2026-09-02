#include "game.hpp"
Game::Game()
    :screen (sf::VideoMode({1200,800}),"DinoRunner"),
    runningPath(createGround())
{
    screen.setFramerateLimit(60);
}
    
sf::RectangleShape Game::createGround(){
    sf::RectangleShape ground({1200,10});
    ground.setPosition({0,395});
    ground.setFillColor(sf::Color::Green);
    return ground;
}

void Game::drawGround(){
    screen.draw(runningPath);
}

void Game::drawPlayerBody(){
    screen.draw(player.getPlayerBody());
}

void Game::run(){
while(screen.isOpen()){
    while(const auto event = screen.pollEvent()){
        if(event->is<sf::Event::Closed>()){
            screen.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::Space){
                player.checkJump();
            }
        }
    }
    screen.clear();
    drawGround();
    drawPlayerBody();
    player.Jump();
    screen.display();
    
}
}