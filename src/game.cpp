#include "game.hpp"
Game::Game()
    :screen (sf::VideoMode({1200,800}),"DinoRunner"),
    runningPath(createGround()),
    collision(false),
    paused(false),
    score(0),
    bestScore(0),
    scoreAwarded(false),
    scoreFont("assets/font.ttf"),
    scoreText(scoreFont, "SCORE: 0", 30),
    bestScoreText(scoreFont, "SCORE: 0", 30)
{
    screen.setFramerateLimit(60);
    scoreText.setPosition({1000.f, 12.f});
    scoreText.setFillColor(sf::Color(248, 250, 252));

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
    reset();
}


void Game::drawScore(){
    scoreText.setString("SCORE " + std::to_string(score));
    screen.draw(scoreText);

    bestScoreText.setString("BEST SCORE " + std::to_string(bestScore));
    screen.draw(bestScoreText);
}

void Game::updateScore(){
    if((enemy.getEnemyBody().getPosition().x < player.getPlayerBody().getPosition().x - enemy.getEnemyBody().getSize().x) 
        &&(!scoreAwarded)){
        score++;
        if(score % 2 == 0){
            enemy.increaseDifficulty(0.5);
        }
        scoreAwarded = true;
    }

    if(enemy.getRespawnedStatus()){
        scoreAwarded = false;
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
            if(bestScore < score){
                bestScore = score;
            }
            pauseGame();
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
                    if(!collision){
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
        updateScore();
        fire.move();
    }

    screen.clear(sf::Color(15, 23, 42));
    drawGround();
    drawPlayerBody();
    drawEnemyBody();
    drawFireballBody();
    drawScore();
    screen.display();
}
}