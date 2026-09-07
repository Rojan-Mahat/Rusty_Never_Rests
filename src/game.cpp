#include "game.hpp"
#include <fstream>
#include <cstdlib>
#include <cmath>
#include<random>
#include<vector>
#include <SFML/Audio.hpp>
#include "world.hpp"

Game::Game()
    :screen (sf::VideoMode({1200,800}),"Rusty Never Rests"),
    backgroundTexture("assets/background/scene/background.png"),
    backgroundSprite(backgroundTexture),

    runningPath(createGround()),
    paused(false),
    score(0),
    bestScore(0),
    onLand(true),
    oxygen(100),

    enemy1Texture("assets/sprites/enemies/enemy1.png"),
    enemy2Texture("assets/sprites/enemies/enemy2.png"),
    enemies({Enemy({1160,World::ENEMY_Y},-6.5,enemy1Texture,{-43.f, 5.f},{70.f, 48.f},-30.f),Enemy({1700,World::ENEMY_Y},-6.5,enemy2Texture,{-43.f, -9.f},{65.f, 55.f},-20.f)}),

    power({createRandom(2500,8000),World::POWER_Y},createRandom(-15,-10),Powerup::Type::Health),
    scoreFont("assets/fonts/font.ttf"),
    scoreText(scoreFont, "SCORE 0", 30),
    bestScoreText(scoreFont, "SCORE 0", 30),
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
    waterZapFlashTimer(0.f),
    groundEffectTimer(0.f),
   
    collisionBuffer("assets/audio/sfx/hazards/collision.mp3"),
    collisionSound(collisionBuffer),

    fireBuffer("assets/audio/sfx/hazards/fire.mp3"),
    fireSound(fireBuffer),

    electricBuffer("assets/audio/sfx/hazards/electricity.mp3"),
    electricSound(electricBuffer),

    powerupBuffer("assets/audio/sfx/powerups/powerup.mp3"),
    powerupSound(powerupBuffer),

    godModeBuffer("assets/audio/sfx/powerups/godmode.mp3"),
    godModeSound(godModeBuffer),

    splashBuffer("assets/audio/sfx/player/splash.mp3"),
    splashSound(splashBuffer),

    gameOverBuffer("assets/audio/sfx/game/gameover.mp3"),
    gameOverSound(gameOverBuffer),

    electricFieldBuffer("assets/audio/sfx/hazards/electricField.mp3"),
    electricFieldSound(electricFieldBuffer),

    gameplayMusic("assets/audio/music/gameplay.mp3"),
    gameOverMusic("assets/audio/music/game_over.mp3"),
    menuMusic("assets/audio/music/Start.mp3"),

    fireCollisionBuffer("assets/audio/sfx/hazards/fireCollision.wav"),
    fireCollisionSound(fireCollisionBuffer),

    shieldBuffer("assets/audio/sfx/powerups/shieldLoop.wav"),
    shieldSound(shieldBuffer),

    electricModeBuffer("assets/audio/sfx/powerups/electricModeLoop.wav"),
    electricModeSound(electricModeBuffer),

    flightModeBuffer("assets/audio/sfx/player/player_flying.mp3"),
    flightModeSound(flightModeBuffer),
    
    underwaterTexture("assets/background/water/underwater_sheet.png"),
    underwaterFrame(0),
    underwaterTimer(0.f),
    underwaterFrameDuration(0.25f),

    farTexture("assets/background/parallax/far.png"),
    farA(farTexture),
    farB(farTexture),
    farSpeed(12.f),
    farWidth(0.f),

    midTexture("assets/background/parallax/mid.png"),
    midA(midTexture),
    midB(midTexture),
    midSpeed(30.f),
    midWidth(0.f),

    closeTexture("assets/background/parallax/close.png"),
    closeA(closeTexture),
    closeB(closeTexture),
    closeSpeed(65.f),
    closeWidth(0.f),

    healthBarTexture("assets/sprites/ui/healthBar.png"),
    healthBarSprite(healthBarTexture),

    menuState(MenuState::Main),
    menuTitleText(scoreFont, "RUSTY NEVER RESTS!", 48),
    playText(scoreFont, "PLAY..", 38),
    challengeText(scoreFont, "TRY BEATING THE SCORE", 24),
    menuBestScoreText(scoreFont, "BEST SCORE: 0", 28),
    howToText(scoreFont, "HOW TO", 35),
    settingsText(scoreFont, "SETTINGS", 35),
    restartText(scoreFont, "RESTART", 30),
    menuFlavorText(scoreFont, "RUN  |    DIVE   |   SURVIVE", 30),
    previousMenuState(MenuState::Main),

    howToBodyText(scoreFont,
        "SPACE / W / UP    JUMP / SURFACE\n\n"
        "S / DOWN          DIVE\n\n"
        "ENTER             EMERGENCY SHIELD\n\n"
        "P                 PAUSE\n\n"
        "AVOID ENEMIES AND FIRE\n"
        "WATCH YOUR OXYGEN UNDERWATER\n"
        "COLLECT POWERUPS AND BEAT YOUR BEST SCORE",
        23
    ),
    backText(scoreFont, "BACK", 30),
    musicText(scoreFont, "MUSIC: ON", 30),
    musicEnabled(true),
    sfxText(scoreFont, "SFX: ON", 30),
    sfxEnabled(true),

    gameOverTitleText(scoreFont, "YOU LOSE!", 60),
    finalScoreText(scoreFont, "YOUR SCORE: 0", 35),
    beatScoreText(scoreFont, "CAN YOU BEAT THAT SCORE?", 30),
    playAgainText(scoreFont, "PLAY AGAIN", 40),
    quitText(scoreFont, "QUIT", 30),
    gameOverQuoteText(scoreFont,"ONE MORE RUN. MAKE IT COUNT.",30)

{
    loadBestScore();
    backgroundSprite.setScale({1200.f / 1536.f,800.f / 1024.f});
    backgroundSprite.setPosition({0.f, 35.f});
    screen.setFramerateLimit(60);
    scoreText.setPosition({1000.f, 12.f});

    powerDurationText.setPosition({470.f, 55.f});
    powerDurationText.setFillColor(sf::Color(56, 189, 248));

    bestScoreText.setPosition({50,12.f});
    bestScoreText.setFillColor(sf::Color(248, 250, 252));

    oxygenText.setPosition({950.f, 740.f});
    oxygenText.setFillColor(sf::Color(56, 189, 248));

    electricityTime = electricityTimeGenerator();
    initial_speed_enemy.resize(enemies.size());

    collisionSound.setVolume(70.f);

    gameplayMusic.setLooping(true);
    gameplayMusic.setVolume(70.f);
    splashSound.setVolume(50.f);

    gameOverMusic.setLooping(true);
    gameOverMusic.setVolume(60.f);
    menuMusic.setLooping(true);
    menuMusic.setVolume(70.f);
    menuMusic.play();

    shieldSound.setLooping(true);
    electricModeSound.setLooping(true);

    flightModeSound.setLooping(true);
    fireCollisionSound.setLooping(false);

    float farScale = World::SCREEN_WIDTH /static_cast<float>(farTexture.getSize().x);
    farA.setScale({farScale,farScale});
    farB.setScale({farScale,farScale});
    farWidth = static_cast<float>(farTexture.getSize().x )* farScale;
    farA.setPosition({0.f,80.f});
    farB.setPosition({farWidth,80.f});

    float midScale = World::SCREEN_WIDTH /static_cast<float>(midTexture.getSize().x);
    midA.setScale({midScale,midScale});
    midB.setScale({midScale,midScale});
    midWidth = static_cast<float>(midTexture.getSize().x)* midScale;
    midA.setPosition({0.f,125.f});
    midB.setPosition({midWidth,125.f});

    float closeScale = World::SCREEN_WIDTH /static_cast<float>(closeTexture.getSize().x);
    closeScale+=0.07f;
    closeA.setScale({closeScale,closeScale});
    closeB.setScale({closeScale,closeScale});
    closeWidth = static_cast<float>(closeTexture.getSize().x) * 0.62f;
    closeA.setPosition({0.f,105.f});
    closeB.setPosition({closeWidth,105.f});


    for(int i = 0; i < 9; i++){
        underwaterSprites.emplace_back(underwaterTexture);
        sf::Sprite& sprite = underwaterSprites.back();
        sprite.setTextureRect(sf::IntRect({0,0},{313,380}));
        sprite.setScale({0.46f,0.76f});
        sprite.setPosition({i * 144.f, World::WATER_Y});
        sprite.setColor(sf::Color(255,255,255,145));
    }

    menuTitleText.setPosition({340.f, 150.f});
    menuTitleText.setFillColor(sf::Color(120, 220, 255));
    playText.setPosition({540.f, 260.f});

    challengeText.setPosition({75.f, 650.f});
    menuBestScoreText.setPosition({75.f, 685.f});
    challengeText.setFillColor(sf::Color(250, 204, 21));
    menuBestScoreText.setFillColor(sf::Color(249, 115, 22));

    howToText.setPosition({525.f, 450.f});
    settingsText.setPosition({510.f, 550.f});

    restartText.setPosition({515.f, 435.f});
    menuFlavorText.setPosition({445.f, 360.f});
    menuFlavorText.setFillColor(sf::Color(80, 200, 230));
    howToBodyText.setPosition({310.f, 245.f});

    howToBodyText.setFillColor(sf::Color(220, 235, 245));
    backText.setPosition({550.f, 675.f});
    backText.setFillColor(sf::Color(250, 204, 21));
    musicText.setPosition({500.f, 330.f});
    musicText.setFillColor(sf::Color(120, 220, 255));
    sfxText.setPosition({500.f, 390.f});
    sfxText.setFillColor(sf::Color(250, 204, 21));

    gameOverTitleText.setPosition({445.f, 150.f});
    gameOverTitleText.setFillColor(sf::Color(248, 90, 90));
    finalScoreText.setPosition({465.f, 270.f});
    finalScoreText.setFillColor(sf::Color(250, 204, 21));
    beatScoreText.setPosition({410.f, 350.f});
    beatScoreText.setFillColor(sf::Color(180, 225, 255));
    gameOverQuoteText.setPosition({350.f, 630.f});
    gameOverQuoteText.setFillColor(sf::Color(120, 220, 255));
    playAgainText.setPosition({500.f, 420.f});
    playAgainText.setFillColor(sf::Color(90, 230, 150));
    quitText.setPosition({565.f, 510.f});
    quitText.setFillColor(sf::Color(210, 215, 225));

    healthBarSprite.setScale({0.35f, 0.35f});
    healthBarSprite.setPosition({690.f, 10.f});

}
    
sf::RectangleShape Game::createGround(){
    sf::RectangleShape ground({ World::SCREEN_WIDTH,World::GROUND_HEIGHT});
    ground.setPosition({0,World::GROUND_Y});
    ground.setFillColor(sf::Color(35, 38, 42));
    ground.setOutlineThickness(3.f);
    ground.setOutlineColor(sf::Color(40, 210, 225));
    return ground;
}

void Game::drawMenu(){
    sf::RectangleShape overlay({World::SCREEN_WIDTH,World::SCREEN_HEIGHT});
    overlay.setFillColor(sf::Color(5, 10, 20, 150));
    screen.draw(overlay);

    if(menuState==MenuState::Main){
        menuTitleText.setString("RUSTY NEVER RESTS!");
        menuTitleText.setPosition({340.f, 150.f});
        playText.setString("PLAY..");
        playText.setPosition({540.f, 260.f});
        howToText.setPosition({525.f, 450.f});
        settingsText.setPosition({510.f, 550.f});
        
        screen.draw(menuTitleText);
        screen.draw(playText);
        screen.draw(menuFlavorText);
        menuBestScoreText.setString("BEST SCORE: " + std::to_string(bestScore));
        screen.draw(challengeText);
        screen.draw(menuBestScoreText);
        screen.draw(howToText);
        screen.draw(settingsText);
    }
    else if(menuState == MenuState::Pause){
        playText.setString("RESUME");
        screen.draw(restartText);
        auto centerText = [](sf::Text& text, float y){
            sf::FloatRect bounds = text.getLocalBounds();
            float x =(World::SCREEN_WIDTH - bounds.size.x) / 2.f - bounds.position.x;
            text.setPosition({x, y});
        };
        centerText(playText,     300.f);
        centerText(restartText,  380.f);
        centerText(howToText,    460.f);
        centerText(settingsText, 540.f);
        screen.draw(restartText);
        screen.draw(playText);
        screen.draw(howToText);
        screen.draw(settingsText);
    }

    else if(menuState == MenuState::HowTo){
        menuTitleText.setString("HOW TO PLAY");
        menuTitleText.setPosition({430.f, 140.f});
        screen.draw(menuTitleText);
        screen.draw(howToBodyText);
        screen.draw(backText);
    }

    else if(menuState == MenuState::Settings){
        menuTitleText.setString("SETTINGS");
        menuTitleText.setPosition({455.f, 140.f});
        if(musicEnabled)
            musicText.setString("MUSIC: ON");
        else
            musicText.setString("MUSIC: OFF");
        if(sfxEnabled)
            sfxText.setString("SFX: ON");
        else
            sfxText.setString("SFX: OFF");
        screen.draw(menuTitleText);
        screen.draw(musicText);
        screen.draw(sfxText);
        screen.draw(backText);
    }

    else if(menuState == MenuState::GameOver){
        gameOverTitleText.setString("YOU LOSE!");
        finalScoreText.setString("YOUR SCORE: " + std::to_string(score));
        screen.draw(gameOverTitleText);
        screen.draw(finalScoreText);
        screen.draw(beatScoreText);
        screen.draw(gameOverQuoteText);
        screen.draw(playAgainText);
        screen.draw(quitText);
    }
}

void Game::loadBestScore(){
    const char* home = std::getenv("HOME");
    if(home == nullptr)
        return;
    std::string path = std::string(home) + "/.rusty_never_rests_best_score";
    std::ifstream file(path);
    if(file.is_open())
        file >> bestScore;
}

void Game::saveBestScore(){
    const char* home = std::getenv("HOME");
    if(home == nullptr)
        return;
    std::string path =std::string(home) + "/.rusty_never_rests_best_score";
    std::ofstream file(path);
    if(file.is_open())
        file << bestScore;
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
    player.clearAura();
}

void Game::drawBackground(){
    screen.draw(backgroundSprite);
}

void Game::drawGround(){
    screen.draw(runningPath);
}

void Game::drawPlayerBody(){
    if(player.isAuraVisible())
        screen.draw(player.getAuraSprite());
    screen.draw(player.getPlayerSprite());
}

void Game::drawEnemyBody(){
    for(const auto& enemy:enemies){
        screen.draw(enemy.getEnemySprite());
    }
}

void Game::drawFireballBody(){
     screen.draw(fire.getFireballSprite());
}

void Game::drawPowerUps(){
    screen.draw(power.getPowerSprite());
}

void Game::pauseGame(){
    paused = true;
    player.pauseAnimation();
    shieldSound.pause();
    electricModeSound.pause();
    flightModeSound.pause();
}

void Game::resumeGame(){
    paused = false;
    player.resumeAnimation();
    if(sfxEnabled){
        if(flightActive){
            flightModeSound.play();
        }
        else if(!godActive && electricalShield){
            electricModeSound.play();
        }
        else if(!godActive && collisionShield){
            shieldSound.play();
        }
    }
}

void Game::restartGame(){
    for(auto& enemy : enemies)
        enemy.reset();
    player.reset();
    player.resetAnimation();
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

void Game::updateGroundEffect(float dt){
    groundEffectTimer += dt;
    float pulse =(std::sin(groundEffectTimer * 2.5f) + 1.f) / 2.f;
    if(!electricWarning){
        int brightness =static_cast<int>(18.f + pulse * 20.f);
        runningPath.setFillColor(sf::Color(brightness,brightness + 4,brightness + 8));
        int cyan =static_cast<int>(150.f + pulse * 90.f);
        runningPath.setOutlineColor(sf::Color(30,cyan,255));
    }
    else{
        int phase = static_cast<int>(groundEffectTimer * 18.f);
        if(phase%2 == 0){
            runningPath.setFillColor(sf::Color(20, 90, 120));
            runningPath.setOutlineColor(sf::Color(80, 220, 255));
        }
        else{
            runningPath.setFillColor(sf::Color(80, 120, 140));

            runningPath.setOutlineColor(sf::Color(230, 250, 255));
        }
    }
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
        waterZapFlashTimer = 0.30f;
        if(!electricalShield){
            electricSound.play();
            player.changeHP(2);
        }
        else{       
            score++;
            if(bestScore < score)
                bestScore = score;
                saveBestScore();
        }
    }
}

void Game::drawElectricWarning(){
    if(!electricWarning){
        return;
    }
    sf::RectangleShape warning({ World::SCREEN_WIDTH,World::SCREEN_HEIGHT - World::WATER_Y});
    warning.setPosition({0.f, World::WATER_Y});
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
        power.getPowerBody().setPosition({createRandom(2500,8000),World::POWER_Y});
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

void Game::updateHealthBattery(){
    int hp = player.getPlayerhp();
    if(hp <= 0)
        healthBarSprite.setTextureRect(sf::IntRect({51,241},{379,225}));
    else if(hp == 1)
        healthBarSprite.setTextureRect(sf::IntRect({470,241},{381,225}));
    else if(hp == 2)
        healthBarSprite.setTextureRect(sf::IntRect({892,241},{381,225}));
    else if(hp == 3)
        healthBarSprite.setTextureRect(sf::IntRect({1314,241},{383,225}));
    else
        healthBarSprite.setTextureRect(sf::IntRect({1750,241},{379,225}));

    sf::FloatRect bounds = healthBarSprite.getLocalBounds();
    float x =(World::SCREEN_WIDTH - bounds.size.x * 0.30f) / 2.f;
    healthBarSprite.setPosition({x, 5.f});
}

void Game::drawTexts(){
    updateHealthBattery();
    screen.draw(healthBarSprite);

    scoreText.setString("SCORE " + std::to_string(score));
    screen.draw(scoreText);

    bestScoreText.setString("BEST SCORE " + std::to_string(bestScore));
    screen.draw(bestScoreText);

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
                saveBestScore();
            }
            if(score% 5 == 0 && !godActive){
                for(auto& enemy: enemies)
                    enemy.increaseDifficulty(0.25f);
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

    if(player.getPlayerhp()<=0 && menuState != MenuState::GameOver){
        gameOverSound.play();
        gameplayMusic.stop();
        if(musicEnabled)
            gameOverMusic.play();
        pauseGame();
        menuState = MenuState::GameOver;
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
    player.useShieldAura();
}

void Game::flightMode(){
    flightModeSound.play();
    powerDuration = 10;
    poweredState = true;
    flightActive = true;

    player.cancelJump();
    player.setPlacement({80, 100});

    player.useFlyingAnimation();
    player.useFlightAura();
}

void Game::resistElectricity(){
    electricModeSound.play();
    poweredState = true;
    powerDuration = 15;
    enable_electrical_shield();
    player.useElectricAura();
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
    player.useFlyingAnimation();
    player.useGodAura();
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
    powerUpTimer = 0;
    enable_collision_shield();
    player.useShieldAura();
}

void Game::drawWaterZapEffect(){
    if(waterZapFlashTimer <= 0.f)
        return;
    sf::RectangleShape flash({World::SCREEN_WIDTH,World::SCREEN_HEIGHT - World::WATER_Y});
    flash.setPosition({0.f,World::WATER_Y});

    int phase =static_cast<int>(waterZapFlashTimer * 40.f);

    if(phase % 2 == 0)
        flash.setFillColor(sf::Color(80,220,255,100));
    else
        flash.setFillColor(sf::Color(230,250,255,150));
    screen.draw(flash);
}

void Game::updateUnderwaterAnimation(float dt){
    underwaterTimer += dt;
    if(underwaterTimer >= underwaterFrameDuration){
        underwaterTimer -= underwaterFrameDuration;
        underwaterFrame++;
        if(underwaterFrame >= 8)
            underwaterFrame = 0;
    }
    int column = underwaterFrame % 4;
    int row = underwaterFrame / 4;

     for(auto& sprite : underwaterSprites){
        sprite.setTextureRect(sf::IntRect({column * 313, row * 380},{313,380}));
    }
}

void Game::drawUnderwaterAnimation(){
    for(const auto& sprite : underwaterSprites){
        screen.draw(sprite);
    }
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

void Game::setSfxEnabled(bool enabled){
    if(!enabled){
        collisionSound.setVolume(0.f);
        fireSound.setVolume(0.f);
        electricSound.setVolume(0.f);
        powerupSound.setVolume(0.f);
        godModeSound.setVolume(0.f);
        splashSound.setVolume(0.f);
        gameOverSound.setVolume(0.f);
        electricFieldSound.setVolume(0.f);
        fireCollisionSound.setVolume(0.f);
        shieldSound.setVolume(0.f);
        electricModeSound.setVolume(0.f);
        flightModeSound.setVolume(0.f);
        fire.disableSFX();
        player.disableSFX();
    }
    else{
        collisionSound.setVolume(70.f);
        splashSound.setVolume(50.f);
        fireSound.setVolume(100.f);
        electricSound.setVolume(100.f);
        powerupSound.setVolume(100.f);
        godModeSound.setVolume(100.f);
        gameOverSound.setVolume(100.f);
        electricFieldSound.setVolume(100.f);
        fireCollisionSound.setVolume(100.f);
        shieldSound.setVolume(100.f);
        electricModeSound.setVolume(100.f);
        flightModeSound.setVolume(100.f);
        fire.enableSFX();
        player.disableSFX();
    }
}

void Game::updateFarParallax(float dt){
    float distance = farSpeed*dt;
    farA.move({-distance, 0.f});
    farB.move({-distance, 0.f});

    if(farA.getPosition().x <= -farWidth)
        farA.setPosition({farB.getPosition().x + farWidth, farA.getPosition().y});

    if(farB.getPosition().x <= -farWidth)
        farB.setPosition({farA.getPosition().x+farWidth , farB.getPosition().y}); 
}

void Game::updateMidParallax(float dt){
    float distance = midSpeed*dt;
    midA.move({-distance, 0.f});
    midB.move({-distance, 0.f});

    if(midA.getPosition().x <= -midWidth)
        midA.setPosition({midB.getPosition().x + midWidth, midA.getPosition().y});

    if(midB.getPosition().x <= -midWidth)
        midB.setPosition({midA.getPosition().x + midWidth , midB.getPosition().y}); 
}

void Game::updateCloseParallax(float dt){
    float distance = closeSpeed*dt;
    closeA.move({-distance, 0.f});
    closeB.move({-distance, 0.f});

    if(closeA.getPosition().x <= -closeWidth)
        closeA.setPosition({closeB.getPosition().x + closeWidth, closeA.getPosition().y});

    if(closeB.getPosition().x <= -closeWidth)
        closeB.setPosition({closeA.getPosition().x+closeWidth , closeB.getPosition().y}); 
}


void Game::drawParallax(){
    screen.draw(farA);
    screen.draw(farB);
    screen.draw(midA);
    screen.draw(midB);
    screen.draw(closeA);
    screen.draw(closeB);
}

void Game::run(){
    while(screen.isOpen()){
        float dt = frameClock.restart().asSeconds();
        while(const auto event = screen.pollEvent()){
            if(event->is<sf::Event::Closed>()){
                screen.close();
            }

            if(const auto* mousePressed =event->getIf<sf::Event::MouseButtonPressed>()){
                if(mousePressed->button == sf::Mouse::Button::Left){
                    sf::Vector2i mousePixel =sf::Mouse::getPosition(screen);
                    sf::Vector2f mousePosition = screen.mapPixelToCoords(mousePixel);
                        if(menuState == MenuState::Main && playText.getGlobalBounds().contains(mousePosition)){
                            menuState = MenuState::Hidden;
                            restartGame();
                            menuMusic.stop();
                            if(musicEnabled)
                                gameplayMusic.play();
                            
                        }
                        else if(menuState == MenuState::Pause && playText.getGlobalBounds().contains(mousePosition)){
                            resumeGame();
                            menuState = MenuState::Hidden;
                            menuMusic.stop();
                            if(musicEnabled)
                                gameplayMusic.play();
                        }
                        else if(menuState == MenuState::Pause &&restartText.getGlobalBounds().contains(mousePosition)){
                            restartGame();
                            menuState = MenuState::Hidden;
                            menuMusic.stop();
                            gameplayMusic.stop();
                            if(musicEnabled)
                                gameplayMusic.play();
                        }
                        else if((menuState == MenuState::Main || menuState == MenuState::Pause) && howToText.getGlobalBounds().contains(mousePosition)){
                            previousMenuState = menuState;
                            menuState = MenuState::HowTo;
                        }
                        else if((menuState == MenuState::HowTo || menuState == MenuState::Settings) && backText.getGlobalBounds().contains(mousePosition)){
                            menuState = previousMenuState;
                        }
                        else if((menuState == MenuState::Main || menuState == MenuState::Pause) && settingsText.getGlobalBounds().contains(mousePosition)){
                            previousMenuState = menuState;
                            menuState = MenuState::Settings;
                        }
                        else if(menuState == MenuState::Settings && musicText.getGlobalBounds().contains(mousePosition)){
                            musicEnabled = !musicEnabled;
                            if(musicEnabled){
                                gameplayMusic.pause();
                                gameOverMusic.pause();
                                menuMusic.play();
                                
                            }
                            else{
                                gameplayMusic.pause();
                                gameOverMusic.pause();
                                menuMusic.pause();
                            }
                        }
                        else if(menuState == MenuState::Settings && sfxText.getGlobalBounds().contains(mousePosition)){
                            sfxEnabled = !sfxEnabled;
                            setSfxEnabled(sfxEnabled);
                        }
                        else if(menuState == MenuState::GameOver && playAgainText.getGlobalBounds().contains(mousePosition)){
                            gameOverMusic.stop();
                            restartGame();
                            menuState = MenuState::Hidden;
                            if(musicEnabled)
                                gameplayMusic.play();
                        }
                        else if(menuState == MenuState::GameOver && quitText.getGlobalBounds().contains(mousePosition)){
                            screen.close();
                        }
                }

            }
        

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if((keyPressed->code == sf::Keyboard::Key::Space)||(keyPressed->code == sf::Keyboard::Key::Up)||((keyPressed->code == sf::Keyboard::Key::W))){
                    if(!paused && menuState == MenuState::Hidden){
                        if(!flightActive && !godActive && !player.isPlayerJumping()&& onLand){
                            player.checkJump();
                        }

                        if(!flightActive && !godActive && !onLand){
                            onLand = true;
                            stateChanger();
                            player.useRunningAnimation();
                            splashSound.play();
                        }

                        if(flightActive || godActive){
                            player.setPlacement({80,100});
                        }
                    } 
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::P){
                    if(menuState == MenuState::Hidden && !paused){
                        pauseGame();
                        menuState = MenuState::Pause;
                        gameplayMusic.pause();
                        if(musicEnabled)
                            menuMusic.play();
                    }
                    else if(menuState == MenuState::Pause){
                        resumeGame();
                        menuState = MenuState::Hidden;
                        menuMusic.stop();
                        if(musicEnabled)
                            gameplayMusic.play();
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if((keyPressed->code == sf::Keyboard::Key::S ||
                    keyPressed->code == sf::Keyboard::Key::Down) && !paused && menuState == MenuState::Hidden){
                    if(flightActive || godActive){
                        player.setPlacement({80,215});
                    }
                    else if(onLand && player.isPlayerJumping()){
                        player.setPlacement({80,World::PLAYER_Y});
                        player.cancelJump();
                        player.useRunningAnimation();
                    }
                    else if(onLand){
                        onLand = false;
                        stateChanger();
                        player.useSwimmingAnimation();
                        splashSound.play();
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if(((keyPressed->code == sf::Keyboard::Key::W)) && (!paused && menuState == MenuState::Hidden)) {
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
                if ((keyPressed->code == sf::Keyboard::Key::Enter) && (!paused && menuState == MenuState::Hidden) && (!poweredState) && (onLand)){
                    safety++;
                    if(safety <= 3){
                        shortSafetyMode();
                    }
                }
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::R && !paused && menuState == MenuState::Hidden){
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
                    player.setPlacement({80,World::PLAYER_Y});
                    disable_collision_shield();
                    disable_electrical_shield();
                    for(int i = 0 ; i < initial_speed_enemy.size(); i++){
                            enemies[i].setSpeed(initial_speed_enemy[i]);
                        }
                    fire.reset();
                    enable_collision_shield();
                    player.useShieldAura();
                    player.useRunningAnimation();
                    shieldSound.play();
                    powerDuration = 2;
                    }
                }
            
            else if(collisionShield){
                if(powerUpTimer >= powerDuration){
                    poweredState = false;
                    disable_collision_shield();
                    player.clearAura();
                    shieldSound.stop();
                    powerUpTimer = 0;
                }
            }

            else if(electricalShield){
                if(powerUpTimer >= powerDuration){
                    poweredState = false;
                    disable_electrical_shield();
                    player.clearAura();
                    electricModeSound.stop();
                    powerUpTimer = 0;
                }
            }

            else if(flightActive){
                if(powerUpTimer >= powerDuration){
                    flightActive = false;
                    player.setPlacement({80,World::PLAYER_Y});
                    flightModeSound.stop();
                    enable_collision_shield();
                    shieldSound.play();
                    player.useShieldAura();
                    player.useRunningAnimation();
                    powerDuration = 2;
                    powerUpTimer = 0;
                }
            }
        }

        if(!paused && menuState == MenuState::Hidden){
            updateUnderwaterAnimation(dt);
            updateGroundEffect(dt);
            updateFarParallax(dt);
            updateMidParallax(dt);
            updateCloseParallax(dt);
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
            if(waterZapFlashTimer > 0.f){
                waterZapFlashTimer -= dt;
                if(waterZapFlashTimer < 0.f)
                    waterZapFlashTimer = 0.f;
            }       
        }

        screen.clear(sf::Color(15, 23, 42));
        drawBackground();
        drawParallax();
        drawUnderwaterAnimation();
        drawGround();
        drawElectricWarning();
        player.updateAnimation(dt);
        drawPlayerBody();
        drawEnemyBody();
        drawWaterZapEffect();
        drawFireballBody();
        drawPowerUps();
        drawTexts();
        if(menuState != MenuState::Hidden)
            drawMenu();
        screen.display();
    }
}