#include "game.hpp"
#include <cmath>
#include<random>
Game::Game()
    :screen (sf::VideoMode({1200,800}),"DinoRunner"),
    runningPath(createGround()),
    collision(false),
    paused(false),
    score(0),
    bestScore(0),
    scoreAwarded(false),
    onLand(true),
    oxygen(100),
    scoreFont("assets/font.ttf"),
    scoreText(scoreFont, "SCORE 0", 30),
    bestScoreText(scoreFont, "SCORE 0", 30),
    hpText(scoreFont, "HEALTH  4", 30),
    oxygenText(scoreFont ,"OXYGEN 100",30),
    gameTimer(0),
    burnTimer(0),
    drownTimer(0)
{
    screen.setFramerateLimit(60);
    scoreText.setPosition({1000.f, 12.f});

    hpText.setPosition({500,12.f});
    hpText.setFillColor(sf::Color(34, 197, 94));

    bestScoreText.setPosition({50,12.f});
    bestScoreText.setFillColor(sf::Color(248, 250, 252));

    oxygenText.setPosition({950.f, 740.f});
    oxygenText.setFillColor(sf::Color(56, 189, 248));

    electricityTime = electricityTimeGenerator();
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
    onLand = true;
    burnTimer = 0;
    drownTimer = 0;
    oxygen = 100;
    oxygenBlinkTimer = 0.f;
    oxygenTextVisible = true;
    gameTimer = 0;
    electricWarning = false;
    electricityTime = electricityTimeGenerator();
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

float Game::electricityTimeGenerator(){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> dist(4,7);
    float electricityTime = dist(gen);
    return electricityTime;
}

void Game::electricution(float dt){
    gameTimer += dt;

    if(gameTimer >= electricityTime - 0.65 && gameTimer < electricityTime){
        electricWarning = true;
        warningFlashTimer+=dt;

        if(warningFlashTimer >= 0.1625f){
            warningRed = !warningRed;
            warningFlashTimer = 0.f;
        }
    }

    if(gameTimer >= electricityTime){
        electricWarning = false;
        warningFlashTimer = 0.f;
        warningRed = true;

        gameTimer = 0;
        checkElectricution();
       electricityTime = electricityTimeGenerator();
    }
}

void Game::checkElectricution(){
    if(!onLand){
        player.changeHP(2);
    }
}

void Game::drawElectricWarning(){
    if(!electricWarning){
        return;
    }
    sf::RectangleShape warning({1200.f, 395.f});
    warning.setPosition({0.f, 405.f});
    if(warningRed){
        warning.setFillColor(sf::Color(248, 113, 113, 110));
    }
    else{
        warning.setFillColor(sf::Color(56, 189, 248, 110));
    }
    screen.draw(warning);
}


void Game::trackOxygen(float dt){
    if(onLand){
        drownTimer = 0;
        oxygen+= 10 * dt;
        if(oxygen>=100){
            oxygen = 100;
        }
    }
    
    else{
        if(player.getBurningState()){
            player.setBurningFalse();
            burnTimer = 0;
        }
        oxygen -= 20*dt;
        if(oxygen <= 0){
            oxygen = 0;
            drownTimer+=dt;
            if(drownTimer >= 1){
                player.changeHP(1);
                drownTimer = 0;
            }
        } 
    }
}

void Game::updateOxygenText(float dt){
    // Show oxygen in clean steps:
    // 100, 90, 80, 70...
    int displayedOxygen =
        static_cast<int>(std::ceil(oxygen / 10.f) * 10.f);

    if(displayedOxygen > 100){
        displayedOxygen = 100;
    }

    if(displayedOxygen < 0){
        displayedOxygen = 0;
    }

    oxygenText.setString(
        "OXYGEN " + std::to_string(displayedOxygen)
    );

    // Color coding
    if(oxygen > 60.f){
        oxygenText.setFillColor(
            sf::Color(56, 189, 248)
        );
    }

    else if(oxygen > 30.f){
        oxygenText.setFillColor(
            sf::Color(250, 204, 21)
        );
    }

    else{
        oxygenText.setFillColor(
            sf::Color(248, 113, 113)
        );
    }
    // Blink only while underwater
    if(!onLand){
        oxygenBlinkTimer += dt;
        if(oxygenBlinkTimer >= 0.5f){
            oxygenTextVisible = !oxygenTextVisible;
            oxygenBlinkTimer = 0.f;
        }
    }

    else{
        oxygenBlinkTimer = 0.f;
        oxygenTextVisible = true;
    }
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
    if(oxygenTextVisible){
        screen.draw(oxygenText);
    }
    
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
}

void Game::checkFireContact(){
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
        if(distance <=  fire.getFireballBody().getRadius() && (!player.getBurningState())){
            player.changeHP(1);
            player.setBurningTrue();
        }
    }
    

}

void Game::damageByBurning(float dt){
    burnTimer += dt;
    if(burnTimer >= 2){
        burnTimer = 0;
        player.changeHP(1);
    }
}

void Game::checkPlayer(float dt){
    if(player.getBurningState()){
        damageByBurning(dt);
    }

    if(player.getPlayerhp()<=0){
        pauseGame();
    }
}

void Game::stateChanger(){
    player.changeWorld();
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
    float dt = frameClock.restart().asSeconds();
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
            if(((keyPressed->code == sf::Keyboard::Key::S) ||(keyPressed->code == sf::Keyboard::Key::Down))&&(!player.isPlayerJumping())&&(onLand)&&(!paused)) {
                onLand = false;
                stateChanger();
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if(((keyPressed->code == sf::Keyboard::Key::W) ||(keyPressed->code == sf::Keyboard::Key::Up))&&(!onLand) && (!paused)) {
                onLand = true;
                stateChanger();
            }
        }

        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
            if (keyPressed->code == sf::Keyboard::Key::R){
                restartGame();
            }
        }

    }
    if(!paused){
        trackOxygen(dt);
        updateOxygenText(dt);
        checkPlayer(dt);
        electricution(dt);
        if(onLand){
            player.Jump();
            enemy.move();
            checkCollision();
            checkFireContact();
            updateScore();
            fire.move();
        }
        else{
            enemy.move();
            fire.move();
            
        }
    }


    screen.clear(sf::Color(15, 23, 42));
    drawGround();
    drawElectricWarning();
    drawPlayerBody();
    drawEnemyBody();
    drawFireballBody();
    drawTexts();
    screen.display();
}
}