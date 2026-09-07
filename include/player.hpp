#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Player{
    private:
    sf::RectangleShape bodyP;
    bool inAir;
    int hp;
    float speed;
    bool isBurning;
    bool animate;


    sf::SoundBuffer jumpBuffer;
    sf::Sound jumpSound;

    sf::SoundBuffer landingBuffer;
    sf::Sound landingSound;

    sf::Texture runTexture;
    sf::Texture jumpTexture;
    sf::Texture flyTexture;
    sf::Texture swimTexture;
    
    sf::Sprite runSprite;

    sf::Texture auraTexture;
    sf::Sprite auraSprite;
    bool auraVisible;

    void syncAuraToBody();


    int currentFrame;
    float animationTimer;
    float frameDuration;

    enum class AnimationState{
        Running,
        Jumping,
        Flying,
        Swimming
    };

    AnimationState animationState;
    void syncSpriteToBody();
    enum class AuraState{
        None,
        Flight,
        Shield,
        Electric,
        God
    };

    AuraState auraState;
    float auraEffectTimer;
    void updateAuraEffect(float dt);

    bool zapped;
    float zapTimer;
    void updateDamageEffects(float dt);
    float effectTimer;

    public:
    Player();
    const sf::RectangleShape& getPlayerBody()const;
    void checkJump();
    void Jump();
    const int getPlayerhp() const;
    void changeHP(int damage);

    void reset();
    const bool isPlayerJumping() const;
    void changeWorld();
    void setBurningTrue();
    void setBurningFalse();
    const bool getBurningState() const;
    void cancelJump();
    void setPlacement(sf::Vector2f coordinates);

    const sf::Sprite& getPlayerSprite() const;
    void updateAnimation(float dt);
    void pauseAnimation();
    void resumeAnimation();
    void resetAnimation();

    void useFlyingAnimation();
    void useSwimmingAnimation();
    void useRunningAnimation();

    const sf::Sprite& getAuraSprite() const;
    bool isAuraVisible() const;

    void useFlightAura();
    void useShieldAura();
    void useElectricAura();
    void useGodAura();
    void clearAura();
    void triggerZapEffect();
    void enableSFX();
    void disableSFX();

};