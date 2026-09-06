#include "game.hpp"
#include <cmath>
#include<random>
#include<vector>
#include <SFML/Audio.hpp>

Game::Game()
    :screen (sf::VideoMode({1200,800}),"DinoRunner"),
    runningPath(createGround()),
    paused(false),
    score(0),
    bestScore(0),
    onLand(true),
    oxygen(100),
    enemies({Enemy({1160,295},-6.5),Enemy({1700,295},-6.5)}),
    power({createRandom(2500,8000),370},createRandom(-15,-10),Powerup::Type::Health),
    scoreFont("assets/font.ttf"),
    scoreText(scoreFont, "SCORE 0", 30),
    bestScoreText(scoreFont, "SCORE 0", 30),
    hpText(scoreFont, "HEALTH  4", 30),
    oxygenText(scoreFont ,"OXYGEN 100",30),
    powerDurationText(scoreFont ,"Power Time 0",30),
    gameTimer(0),
    burnTimer(0),
    drownTimer(0),
    powerUpTimer(0),
    collisionShield(false),
    electricalShield(false),
    poweredState(false),
    flightActive(false),
    godActive(false),
    summonGod(0),
    powerDuration(0),
    safety(0),

   
    collisionBuffer("assets/audio/collision.mp3"),
    collisionSound(collisionBuffer),

    fireBuffer("assets/audio/fire.mp3"),
    fireSound(fireBuffer),

    electricBuffer("assets/audio/electricity.mp3"),
    electricSound(electricBuffer),

    powerupBuffer("assets/audio/powerup.mp3"),
    powerupSound(powerupBuffer),

    godModeBuffer("assets/audio/godmode.mp3"),
    godModeSound(godModeBuffer),

    splashBuffer("assets/audio/splash.mp3"),
    splashSound(splashBuffer),

    gameOverBuffer("assets/audio/gameover.mp3"),
    gameOverSound(gameOverBuffer),

    electricFieldBuffer("assets/audio/electricField.mp3"),
    electricFieldSound(electricFieldBuffer),

    gameplayMusic("assets/audio/gameplay.mp3"),
    gameOverMusic("assets/audio/game_over.mp3"),

    fireCollisionBuffer("assets/audio/fireCollision.wav"),
    fireCollisionSound(fireCollisionBuffer),

    shieldBuffer("assets/audio/shieldLoop.wav"),
    shieldSound(shieldBuffer),

    electricModeBuffer("assets/audio/electricModeLoop.wav"),
    electricModeSound(electricModeBuffer),

    flightModeBuffer("assets/audio/flightMode.wav"),
    flightModeSound(flightModeBuffer)

{
    screen.setFramerateLimit(60);
    scoreText.setPosition({1000.f, 12.f});

    powerDurationText.setPosition({470.f, 55.f});
    powerDurationText.setFillColor(sf::Color(56, 189, 248));

    hpText.setPosition({500,12.f});
    hpText.setFillColor(sf::Color(34, 197, 94));

    bestScoreText.setPosition({50,12.f});
    bestScoreText.setFillColor(sf::Color(248, 250, 252));

    oxygenText.setPosition({950.f, 740.f});
    oxygenText.setFillColor(sf::Color(56, 189, 248));

    electricityTime = electricityTimeGenerator();
    initial_speed_enemy.resize(enemies.size());

    collisionSound.setVolume(70.f);

    gameplayMusic.setLooping(true);
    gameplayMusic.setVolume(70.f);
    gameplayMusic.play();
    splashSound.setVolume(50.f);

    gameOverMusic.setLooping(false);
    shieldSound.setLooping(true);
    electricModeSound.setLooping(true);

    flightModeSound.setLooping(false);
    fireCollisionSound.setLooping(false);
}
    
sf::RectangleShape Game::createGround(){
    sf::RectangleShape ground({1200,10});
    ground.setPosition({0,395});
    ground.setFillColor(sf::Color(100, 116, 139));
    return ground;
}

void Game::reset(){
    paused = false;
    score = 0;
    onLand = true;
    burnTimer = 0;
    drownTimer = 0;
    oxygen = 100;
    oxygenBlinkTimer = 0.f;
    oxygenTextVisible = true;
    gameTimer = 0;
    electricWarning = false;
    electricityTime = electricityTimeGenerator();
    summonGod = 0;
    poweredState = false;
    powerUpTimer = 0;
    collisionShield = false;
    electricalShield = false;
    flightActive = false;
    godActive = false;
    reStartPowerUps();
    fireSound.stop();
    shieldSound.stop();
    electricModeSound.stop();
    flightModeSound.stop();
    gameOverMusic.stop();
    safety = 0;
}

void Game::drawGround(){
    screen.draw(runningPath);
}

void Game::drawPlayerBody(){
    screen.draw(player.getPlayerBody());
}

void Game::drawEnemyBody(){
    for(const auto& enemy:enemies)
        screen.draw(enemy.getEnemyBody());
}

void Game::drawFireballBody(){
     screen.draw(fire.getFireballBody());
}

void Game::drawPowerUps(){
    screen.draw(power.getPowerBody());
}

void Game::pauseGame(){
    paused = true;

}

void Game::resumeGame(){
    paused = false;
}

void Game::restartGame(){
    for(auto& enemy : enemies)
        enemy.reset();
    player.reset();
    fire.reset();
    reset();
}

float Game::createRandom(int a , int b){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> distX(a,b);
    float x = distX(gen);
    return x;
}


float Game::electricityTimeGenerator(){
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_real_distribution<float> dist(4,7);
    float electricityTime = dist(gen);
    return electricityTime;
}

void Game::setRandomType(){
    summonGod++;
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<int> dist(1,4);
    int chosen = dist(gen);
    
    if(chosen == 1){
        power.setType(Powerup::Type::Health);
        power.getPowerBody().setFillColor(sf::Color(34, 197, 94));
    }

    if(chosen == 2){
        power.setType(Powerup::Type::Shield);
        power.getPowerBody().setFillColor(sf::Color(59, 130, 246));
    }

    if(chosen == 3){
        power.setType(Powerup::Type::Flight);
        power.getPowerBody().setFillColor(sf::Color(168, 85, 247));
    }

    if(chosen == 4){
        power.setType(Powerup::Type::ElectricResist);
        power.getPowerBody().setFillColor(sf::Color(250, 204, 21));
    }

    if(summonGod % 4 == 0){
        power.setType(Powerup::Type::GodMode);
        power.getPowerBody().setFillColor(sf::Color(249, 115, 22));
    }
}

void Game::electricution(float dt){
    gameTimer += dt;

    if(gameTimer >= electricityTime - 0.65 && gameTimer < electricityTime){
        if(!electricWarning){
            electricFieldSound.play();
        }

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
        if(!electricalShield){
            electricSound.play();
            player.changeHP(2);
        }
        else{       
            score++;
            if(bestScore < score)
                bestScore = score;
        }
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

void Game::reStartPowerUps(){
    if(!poweredState){
        power.setRespawn(1);
        power.getPowerBody().setPosition({createRandom(2500,8000),370});
        power.setPowerSpeed(createRandom(-15,-10));
        setRandomType();
        power.setPowerAddStatus(0);
        power.setBounds(0);
    }
}

void Game::runPowerUps(){
    if(!power.getIfOutOfBounds()){
        power.setRespawn(0);
        power.move();
    }
    else{
        reStartPowerUps();
    }
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
            fireSound.stop();
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
    int displayedOxygen = static_cast<int>(std::ceil(oxygen / 10.f) * 10.f);

    if(displayedOxygen > 100)
        displayedOxygen = 100;

    if(displayedOxygen < 0)
        displayedOxygen = 0;

    oxygenText.setString("OXYGEN " + std::to_string(displayedOxygen));

    if(oxygen > 60.f)
        oxygenText.setFillColor(sf::Color(56, 189, 248));
    
    else if(oxygen > 30.f)
        oxygenText.setFillColor(sf::Color(250, 204, 21));

    else
        oxygenText.setFillColor(sf::Color(248, 113, 113));
    
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
    if(poweredState){
        int secondsLeft = static_cast<int>(std::ceil(powerDuration - powerUpTimer));
        std::string powerName;
        if(godActive)
            powerName = "GOD MODE";

        else if(flightActive)
            powerName = "FLIGHT";

        else if(collisionShield)
            powerName = "SHIELD";

        else if(electricalShield)
            powerName = "ELECTRIC RESIST";

        powerDurationText.setString(powerName + "  " + std::to_string(secondsLeft) + "s");
        screen.draw(powerDurationText);
    }
    
}

void Game::updateScore(){
    for(auto& enemy: enemies){
        if((enemy.getPassStatus(player.getPlayerBody().getPosition().x - enemy.getEnemyBody().getSize().x))&&(!enemy.getScoreStatus())){
            score++;
            enemy.markScored();
            if(bestScore < score){
                bestScore = score;
            }
            if(score%3 == 0 && !godActive){
                for(auto& enemy: enemies)
                    enemy.increaseDifficulty(0.7);
            }
        }
        if(enemy.getRespawnedStatus()){
                enemy.unmarkScored();
            }
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
            if(!collisionShield){
                fireCollisionSound.play();
                fireSound.play();
                player.setBurningTrue();
            }
        }
    }
}

void Game::damageByBurning(float dt){
    if(!flightActive && !collisionShield){
        burnTimer += dt;
        if(burnTimer >= 1.2){
            burnTimer = 0;
            player.changeHP(1);
        }
    }
}

void Game::checkPlayer(float dt){
    if(player.getBurningState()){
        damageByBurning(dt);
    }

    if(player.getPlayerhp()<=0){
        gameOverSound.play();
        gameOverMusic.play();
        pauseGame();
    }
}

void Game::stateChanger(){
    player.changeWorld();
}


void Game::checkCollision(){
    for(auto& enemy: enemies){
        if((enemy.getEnemyBody().getPosition().x < player.getPlayerBody().getPosition().x - enemy.getEnemyBody().getSize().x) 
        ||(enemy.getEnemyBody().getPosition().x > player.getPlayerBody().getPosition().x+ player.getPlayerBody().getSize().x )){
            enemy.unmarkDamage();
        }
        else{
            if((enemy.getEnemyBody().getPosition().y <= player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y) && (!enemy.getDamageStatus())){
                enemy.markDamage();
                if(!collisionShield){
                    collisionSound.play();
                    player.changeHP(enemy.getEnemyDamage());
                    break;
                }
            }
        }
    }   
}

void Game::PowerCollision(){
    if((power.getPowerBody().getPosition().x <= player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x + (power.getPowerBody().getRadius()))
    &&(power.getPowerBody().getPosition().x >= player.getPlayerBody().getPosition().x - power.getPowerBody().getRadius())
    &&(power.getPowerBody().getPosition().y - power.getPowerBody().getRadius()<= player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y)
    &&(power.getPowerBody().getPosition().y >= player.getPlayerBody().getPosition().y - power.getPowerBody().getRadius())){   
        float dx, dy;
        if(power.getPowerBody().getPosition().x > player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x){
            dx = power.getPowerBody().getPosition().x - (player.getPlayerBody().getPosition().x + player.getPlayerBody().getSize().x);
        }
        else if (power.getPowerBody().getPosition().x < player.getPlayerBody().getPosition().x){
            dx = player.getPlayerBody().getPosition().x - power.getPowerBody().getPosition().x;
        }
        else{
            dx = 0;
        }

        if(power.getPowerBody().getPosition().y < player.getPlayerBody().getPosition().y)
            dy = player.getPlayerBody().getPosition().y - power.getPowerBody().getPosition().y;
        else if((player.getPlayerBody().getPosition().y) + player.getPlayerBody().getSize().y < (power.getPowerBody().getPosition().y))
            dy = power.getPowerBody().getPosition().y - (player.getPlayerBody().getPosition().y + player.getPlayerBody().getSize().y);
        else
            dy = 0;
        float distance = sqrt(dx*dx + dy*dy);
        if(distance <=  power.getPowerBody().getRadius() && (!power.getPowerAddStatus())&& (!poweredState)){
            providePower();
            power.setPowerAddStatus(1);
        }
    }
}

void Game::disable_electrical_shield(){
    electricalShield= false;
}

void Game::enable_electrical_shield(){
    electricalShield = true;
}


void Game::enable_collision_shield(){
    collisionShield = true;
}

void Game::disable_collision_shield(){
     collisionShield = false;
}

void Game::noDamage(){
    shieldSound.play();
    poweredState = true;
    powerDuration = 12;
    enable_collision_shield();
}

void Game::flightMode(){
    flightModeSound.play();
    powerDuration = 10;
    poweredState = true;
    flightActive = true;
    player.cancelJump();
    player.setPlacement({80, 100});
}

void Game::resistElectricity(){
    electricModeSound.play();
    poweredState = true;
    powerDuration = 15;
    enable_electrical_shield();
}

void Game::godMode(){
    godModeSound.play();
    powerDuration = 7;
    int i = 0;
    for(const auto& enemy:enemies){
        initial_speed_enemy[i] = enemy.getSpeed();
        i++;
    }
    player.changeHP(-2);
    poweredState = true;
    godActive = true;
    player.setBurningFalse();
    enable_electrical_shield();
    enable_collision_shield();
    player.cancelJump();
    player.setPlacement({80, 100});;
    for(auto& enemy:enemies)
        enemy.setSpeed(-20);
    fire.setSpeed(-40);
}

void Game::shortSafetyMode(){
    shieldSound.play();
    poweredState = true;
    powerDuration = 3;
    enable_collision_shield();
}



void Game::providePower(){
    if(power.getType()==Powerup::Type::Health){
        powerupSound.play();
        player.changeHP(-4);
    }

    else if(power.getType()==Powerup::Type::Shield){
        powerupSound.play();
        noDamage();
    }

    else if(power.getType()==Powerup::Type::Flight){
        powerupSound.play();
        flightMode();
    }

    else if(power.getType()==Powerup::Type::ElectricResist){
        powerupSound.play();
        resistElectricity();
    }

    else if(power.getType()==Powerup::Type::GodMode){
        powerupSound.play();
        godMode();
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
                        if(!flightActive && !godActive && !player.isPlayerJumping()){
                            player.checkJump();
                        }
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
                    if(!godActive && !flightActive){
                        onLand = false;
                        stateChanger();
                        splashSound.play();
                    }
                    else{
                        player.setPlacement({80,215});
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(((keyPressed->code == sf::Keyboard::Key::W) ||(keyPressed->code == sf::Keyboard::Key::Up)) && (!paused)) {
                    if(!onLand){
                            onLand = true;
                            stateChanger();
                            splashSound.play();
                    }
                    else if(flightActive || godActive){
                        player.setPlacement({80,100});
                    }
                }
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if ((keyPressed->code == sf::Keyboard::Key::Enter) && (!paused) && (!poweredState) && (onLand)){
                    safety++;
                    if(safety <= 3){
                        shortSafetyMode();
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::R){
                    restartGame();
                }
            }

        }
        
        if(poweredState && !paused){
            powerUpTimer+= dt;
            if(godActive){
                if(powerUpTimer >= powerDuration){
                    powerUpTimer = 0;
                    godActive = false;
                    player.setPlacement({80,345});
                    disable_collision_shield();
                    disable_electrical_shield();
                    for(int i = 0 ; i < initial_speed_enemy.size(); i++){
                            enemies[i].setSpeed(initial_speed_enemy[i]);
                        }
                    fire.reset();
                    enable_collision_shield();
                    shieldSound.play();
                    powerDuration = 2;
                    }
                }
            
            else if(collisionShield){
                if(powerUpTimer >= powerDuration){
                    poweredState = false;
                    disable_collision_shield();
                    shieldSound.stop();
                    powerUpTimer = 0;
                }
            }

            else if(electricalShield){
                if(powerUpTimer >= powerDuration){
                    poweredState = false;
                    disable_electrical_shield();
                    electricModeSound.stop();
                    powerUpTimer = 0;
                }
            }

            else if(flightActive){
                if(powerUpTimer >= powerDuration){
                    flightActive = false;
                    player.setPlacement({80,345});
                    flightModeSound.stop();
                    enable_collision_shield();
                    shieldSound.play();
                    powerDuration = 2;
                    powerUpTimer = 0;
                }
            }
        }

        if(!paused){
            trackOxygen(dt);
            updateOxygenText(dt);
            checkPlayer(dt);
            electricution(dt);
            runPowerUps();
            for(std::size_t i = 0; i < enemies.size(); i++){
                std::size_t other = (i == 0) ? 1 : 0;   
                enemies[i].move(enemies[other].getEnemyBody().getPosition().x);
            }
            fire.move();
            if(onLand){
                PowerCollision();
                checkCollision();
                checkFireContact();
                player.Jump();
                updateScore();
            }
        }

        screen.clear(sf::Color(15, 23, 42));
        drawGround();
        drawElectricWarning();
        drawPlayerBody();
        drawEnemyBody();
        drawFireballBody();
        drawPowerUps();
        drawTexts();
        screen.display();
    }
}