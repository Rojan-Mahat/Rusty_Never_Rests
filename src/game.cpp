#include "game.hpp"
#include <cmath>
Game::Game()
    :screen (sf::VideoMode({1200,800}),"DinoRunner"),
    runningPath(createGround()),
    collision(false),
    paused(false),
    score(0),
    bestScore(0),
    scoreAwarded(false),
    burn(false),
    scoreFont("assets/font.ttf"),
    scoreText(scoreFont, "SCORE 0", 30),
    bestScoreText(scoreFont, "SCORE 0", 30),
    hpText(scoreFont, "HEALTH  4", 30)
{
    screen.setFramerateLimit(60);
    scoreText.setPosition({1000.f, 12.f});

    hpText.setPosition({500,12.f});
    hpText.setFillColor(sf::Color(34, 197, 94));

    bestScoreText.setPosition({50,12.f});
    bestScoreText.setFillColor(sf::Color(248, 250, 252));
}
    
sf::RectangleShape Game::createGround(){
    sf::RectangleShape ground({1200,10});
    ground.setPosition({0,395});
    ground.setFillColor(sf::Color(100, 116, 139));
    return ground;
}

void Game::reset(){
    paused = false;
    collision = false;
    score = 0;
    burn = false;
    scoreAwarded = false;
}

void Game::drawGround(){
    screen.draw(runningPath);
}

void Game::drawPlayerBody(){
    screen.draw(player.getPlayerBody());
}

void Game::drawEnemyBody(){
    screen.draw(enemy.getEnemyBody());
}

void Game::drawFireballBody(){
     screen.draw(fire.getFireballBody());
}

void Game::pauseGame(){
    paused = true;

}

void Game::resumeGame(){
    paused = false;
}

void Game::restartGame(){
    enemy.reset();
    player.reset();
    fire.reset();
    reset();
}


void Game::drawTexts(){
    scoreText.setString("SCORE " + std::to_string(score));
    screen.draw(scoreText);

    bestScoreText.setString("BEST SCORE " + std::to_string(bestScore));
    screen.draw(bestScoreText);

    if(player.getPlayerhp()>2){
    hpText.setFillColor(sf::Color(34, 197, 94));
    }
    else if(player.getPlayerhp()==2){
        hpText.setFillColor(sf::Color(250, 204, 21));
    }
    else{
        hpText.setFillColor(sf::Color(248, 113, 113));
    }
    hpText.setString("HEALTH " + std::to_string(player.getPlayerhp()));
    screen.draw(hpText);

}

void Game::updateScore(){
    if((enemy.getEnemyBody().getPosition().x < player.getPlayerBody().getPosition().x - enemy.getEnemyBody().getSize().x) 
        &&(!scoreAwarded)){
        score++;
        if(bestScore < score){
                bestScore = score;
            }
        if(score % 2 == 0){
            enemy.increaseDifficulty(0.5);
        }
        scoreAwarded = true;
    }

    if(enemy.getRespawnedStatus()){
        scoreAwarded = false;
    }

    if(fire.getRespawnedStatus()){
        burn = false;
    }
}

void Game::checkFireContact(){
    if(!burn){
        if(((fire.getFireballBody().getPosition().x >=player.getPlayerBody().getPosition().x - fire.getFireballBody().getRadius())
        && (fire.getFireballBody().getPosition().x <= player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x + fire.getFireballBody().getRadius()))
        &&((fire.getFireballBody().getPosition().y >= player.getPlayerBody().getPosition().y - fire.getFireballBody().getRadius())
        && (fire.getFireballBody().getPosition().y <= player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y + fire.getFireballBody().getRadius())))
        {
        float dx, dy;
        if(fire.getFireballBody().getPosition().x > player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x)
                dx = fire.getFireballBody().getPosition().x - (player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x);
            else if(fire.getFireballBody().getPosition().x < player.getPlayerBody().getPosition().x)
                dx = player.getPlayerBody().getPosition().x - fire.getFireballBody().getPosition().x;
            else
                dx = 0;
                
            if(fire.getFireballBody().getPosition().y < player.getPlayerBody().getPosition().y)
                dy = player.getPlayerBody().getPosition().y - fire.getFireballBody().getPosition().y;
            else if((player.getPlayerBody().getPosition().y) + player.getPlayerBody().getSize().y < (fire.getFireballBody().getPosition().y))
                dy = fire.getFireballBody().getPosition().y - (player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y);
            else
                dy = 0;

            float distance = sqrt(dx*dx + dy*dy);
            if(distance <=  fire.getFireballBody().getRadius()){
                burn = true;
                player.changeHP(fire.getFireDamage());
            }
        }
    }

}

void Game::checkPlayer(){
    if(player.getPlayerhp()<=0){
        pauseGame();
    }
}


void Game::checkCollision(){
    if((enemy.getEnemyBody().getPosition().x < player.getPlayerBody().getPosition().x - enemy.getEnemyBody().getSize().x) 
    ||(enemy.getEnemyBody().getPosition().x > player.getPlayerBody().getPosition().x+ player.getPlayerBody().getSize().x )){
        collision = false;
    }
    else{
        if(enemy.getEnemyBody().getPosition().y <= player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y){
            collision = true;
            player.changeHP(enemy.getEnemyDamage());
        }
    }
}   

void Game::run(){
while(screen.isOpen()){
    while(const auto event = screen.pollEvent()){
        if(event->is<sf::Event::Closed>()){
            screen.close();
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::Space){
                if(paused){
                    if(player.getPlayerhp()!= 0){
                        resumeGame();
                    }
                }
                else{
                    player.checkJump();
                }
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::P){
                pauseGame();
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::R){
                restartGame();
            }
        }

    }
    if(!paused){
        player.Jump();
        enemy.move();
        checkCollision();
        checkPlayer();
        checkFireContact();
        updateScore();
        fire.move();
    }

    screen.clear(sf::Color(15, 23, 42));
    drawGround();
    drawPlayerBody();
    drawEnemyBody();
    drawFireballBody();
    drawTexts();
    screen.display();
}
}