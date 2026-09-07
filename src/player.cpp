#include <SFML/Graphics.hpp>
#include "player.hpp"
#include <SFML/Audio.hpp>
#include "world.hpp"
#include <cmath>
Player::Player()
    :bodyP({World::PLAYER_WIDTH, World::PLAYER_HEIGHT}),
    inAir(false),
    hp(4),
    speed(0.f),
    isBurning(false),
    animate(true),

    jumpBuffer("assets/audio/sfx/player/jumping.mp3"),
    jumpSound(jumpBuffer),

    landingBuffer("assets/audio/sfx/player/landing.mp3"),
    landingSound(landingBuffer),

    runTexture("assets/sprites/player/running/frog_run_sheet.png"),
    jumpTexture("assets/sprites/player/jumping/frog_jump_sheet.png"),
    flyTexture("assets/sprites/player/flying/frog_fly_sheet.png"),
    swimTexture("assets/sprites/player/swimming/frog_swim_sheet.png"),

    runSprite(runTexture),

    currentFrame(0),
    animationTimer(0.f),
    frameDuration(0.04f),

    auraTexture("assets/sprites/player/effects/powered_aura_sheet.png"),
    auraSprite(auraTexture),
    auraVisible(false),

    animationState(AnimationState::Running),
    auraState(AuraState::None),
    auraEffectTimer(0.f),
    zapped(false),
    zapTimer(0.f),
    effectTimer(0.f)

{
    bodyP.setPosition({80,World::PLAYER_Y});
    bodyP.setFillColor(sf::Color(255,0,0,100));
    runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
    runSprite.setScale({0.14f,0.14f});
    runSprite.setOrigin({512.f,1024.f});

    auraSprite.setTextureRect(sf::IntRect({0,0},{627,627}));
    auraSprite.setOrigin({313.5f,313.5f});
    auraSprite.setScale({0.24f,0.24f});
}

const sf::RectangleShape& Player::getPlayerBody()const{
    return bodyP;
}


void Player::setPlacement(sf::Vector2f coordinates){
    bodyP.setPosition(coordinates);
}

void Player::checkJump(){
    if(bodyP.getPosition().y + bodyP.getSize().y ==  World::GROUND_Y && (!inAir)){
        animationState = AnimationState::Jumping;
        runSprite.setTexture(jumpTexture);
        currentFrame = 0;
        animationTimer = 0.f;
        runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
        jumpSound.play();

        speed = -21.5;
        inAir = true;
    }
}

void Player::reset(){
    bodyP.setPosition({80,World::PLAYER_Y});
    hp = 4;
    inAir = false;
    speed = 0.f;
    isBurning = false;

    animationState = AnimationState::Running;
    runSprite.setTexture(runTexture);

    currentFrame = 0;
    animationTimer = 0.f;

    runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
}

void Player::Jump(){
    if(inAir){
        bodyP.move({0.f,speed});
        speed++;

        if(bodyP.getPosition().y + bodyP.getSize().y == World::GROUND_Y){
            useRunningAnimation();
            landingSound.play();

            inAir = false;
            speed = 0.f;
            return;
        }
    }
}

const int Player::getPlayerhp() const{
    return hp;
}

void Player::changeHP(int damage){
    hp = hp - damage;
    if(hp<=0){
        hp = 0;
    }
    if(hp>=4){
        hp = 4;
    }
}

void Player::setBurningTrue(){
   isBurning = true;
}

void Player::setBurningFalse(){
   isBurning = false;
}

const bool Player::getBurningState() const{
    return isBurning;
}

const bool Player::isPlayerJumping() const {
    return inAir;
}

void Player::changeWorld(){
    if(bodyP.getPosition().y <= World::PLAYER_Y + 1.f){
        bodyP.setPosition({80.f, World::PLAYER_SWIM_Y});
    }
    else{
        bodyP.setPosition({80,World::PLAYER_Y});
    }
}

void Player::cancelJump(){
    inAir = false;
    speed = 0.f;
}

const sf::Sprite& Player::getPlayerSprite() const{
    return runSprite;
}

void Player::updateAnimation(float dt){
    syncSpriteToBody();
    syncAuraToBody();
    updateAuraEffect(dt);
    if(animate){
            int frameCount = 16;
            float activeFrameDuration = 0.04f;
            bool loopAnimation = true;

            if(animationState == AnimationState::Jumping){
                frameCount = 20;
                activeFrameDuration = 0.0367f;
                loopAnimation = false;
            }
            else if(animationState == AnimationState::Flying){
                frameCount = 15;
                activeFrameDuration = 0.04f;
                loopAnimation = true;
            }
            else if(animationState == AnimationState::Swimming){
                frameCount = 19;
                activeFrameDuration = 0.04f;
                loopAnimation = true;
            }

             animationTimer += dt;
            if(animationTimer >= activeFrameDuration){
                animationTimer -= activeFrameDuration;
                if(currentFrame < frameCount - 1)
                    currentFrame++;
                else if(loopAnimation)
                    currentFrame = 0;
                
            }
            int column = currentFrame % 4;
            int row = currentFrame / 4;

            runSprite.setTextureRect(sf::IntRect({column * 1024, row * 1024},{1024,1024}));
            updateDamageEffects(dt);
        }
    }

void Player::pauseAnimation(){
    animate = false;
}

void Player::resumeAnimation(){
    animate = true;
}

void Player::resetAnimation(){
    animate = true;
    animationTimer = 0.f;
    currentFrame = 0;
}

void Player::useFlyingAnimation(){
    if(animationState != AnimationState::Flying){
        animationState = AnimationState::Flying;
        runSprite.setTexture(flyTexture);
        currentFrame = 0;
        animationTimer = 0.f;
        runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
    }
}

void Player::useSwimmingAnimation(){
    if(animationState != AnimationState::Swimming){
        animationState = AnimationState::Swimming;
        runSprite.setTexture(swimTexture);
        currentFrame = 0;
        animationTimer = 0.f;
        runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
    }
}

void Player::useRunningAnimation(){
    if(animationState != AnimationState::Running){
        animationState = AnimationState::Running;
        runSprite.setTexture(runTexture);
        currentFrame = 0;
        animationTimer = 0.f;
        runSprite.setTextureRect(sf::IntRect({0,0},{1024,1024}));
    }
}

void Player::syncSpriteToBody(){
    float anchorX = bodyP.getPosition().x + bodyP.getSize().x / 2.f;
    float anchorY = bodyP.getPosition().y + bodyP.getSize().y;
    float offsetX = 0.f;
    float offsetY = 38.f;
    runSprite.setPosition({anchorX + offsetX,anchorY + offsetY});
}

void Player::syncAuraToBody(){
    auraSprite.setPosition({
        runSprite.getPosition().x,
        runSprite.getPosition().y - 65.f
    });
}


const sf::Sprite& Player::getAuraSprite() const{
    return auraSprite;
}

bool Player::isAuraVisible() const{
    return auraVisible;
}


void Player::useFlightAura(){
    auraVisible = true;
    auraState = AuraState::Flight;
    auraEffectTimer = 0.f;
    auraSprite.setTextureRect(sf::IntRect({0,0},{627,627}));
}


void Player::useShieldAura(){
    auraVisible = true;
    auraState = AuraState::Shield;
    auraEffectTimer = 0.f;
    auraSprite.setTextureRect(sf::IntRect({627,0},{627,627}));
}


void Player::useElectricAura(){
    auraVisible = true;
    auraState = AuraState::Electric;
    auraEffectTimer = 0.f;
    auraSprite.setTextureRect(sf::IntRect({0,627},{627,627}));
}


void Player::useGodAura(){
    auraVisible = true;
    auraState = AuraState::God;
    auraEffectTimer = 0.f;
    auraSprite.setTextureRect(sf::IntRect({627,627},{627,627}));
}


void Player::clearAura(){
    auraVisible = false;
    auraState = AuraState::None;
    auraEffectTimer = 0.f;
    auraSprite.setColor(sf::Color::White);
}


void Player::updateAuraEffect(float dt){
    if(!auraVisible)
        return;
    auraEffectTimer += dt;
    if(auraState == AuraState::Flight)
        auraSprite.setColor(sf::Color(255,255,255,35));
    
    else if(auraState == AuraState::Shield){
        float pulse =(std::sin(auraEffectTimer * 4.f) + 1.f) / 2.f;
        int alpha =static_cast<int>(90.f + pulse * 100.f);
        auraSprite.setColor(sf::Color(255,255,255,alpha));
    }

    else if(auraState == AuraState::Electric){
        float pulse =(std::sin(auraEffectTimer * 12.f) + 1.f) / 2.f;
        int alpha =static_cast<int>(80.f + pulse * 175.f);
        auraSprite.setColor(sf::Color(255,255,255,alpha));
    }

    else if(auraState == AuraState::God){
        float pulse =(std::sin(auraEffectTimer * 8.f) + 1.f) / 2.f;
        int alpha =static_cast<int>(140.f + pulse * 115.f);
        auraSprite.setColor(sf::Color(255,255,255,alpha));
    }
}

void Player::triggerZapEffect(){
    zapped = true;
    zapTimer = 0.f;
}

void Player::updateDamageEffects(float dt){
    effectTimer += dt;
    if(zapped){
        zapTimer += dt;
        int phase = static_cast<int>(zapTimer * 30.f);
        if(phase % 2 == 0)
            runSprite.setColor(sf::Color(120,220,255));
        else
            runSprite.setColor(sf::Color::White);

        float shake = (phase % 2 == 0) ? -3.f : 3.f;
        runSprite.move({shake,0.f});
        if(zapTimer >= 0.35f){
            zapped = false;
            zapTimer = 0.f;
            runSprite.setColor(sf::Color::White);
        }
        return;
    }

    if(isBurning){
        int phase =static_cast<int>(effectTimer * 12.f);
        if(phase % 2 == 0)
            runSprite.setColor(sf::Color(255,120,60));
        else
            runSprite.setColor(sf::Color(255,190,100));
        return;
    }
    runSprite.setColor(sf::Color::White);
}

void Player::enableSFX(){
    jumpSound.setVolume(100.0f);
    landingSound.setVolume(100.f);
}

void Player::disableSFX(){
    jumpSound.setVolume(0.f);
    landingSound.setVolume(0.f);
}