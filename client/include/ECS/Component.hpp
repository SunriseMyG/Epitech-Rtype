/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Component
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Component {
public:
    virtual ~Component() = default;
    virtual void update() {}
};

struct PositionComponent : public Component {
    float x, y;
    PositionComponent(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};

struct VelocityComponent : public Component {
    float dx, dy;
    VelocityComponent(float dx = 0.0f, float dy = 0.0f) : dx(dx), dy(dy) {}
};

struct RenderComponent : public Component {
    enum class Type {
        PLAYER,
        BULLET,
        ENEMY,
        BOSS,
        POWERUP,
        EXPLOSION,
        OBSTACLE
    };
    Type type;
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    float width;
    float height;
    
    int currentFrame;
    float animationTimer;
    float frameTime;
    int frameCount;
    sf::IntRect frameRect;
    int frameWidth;
    int frameHeight;
    int currentRow;
    int totalRows;
    bool animationDone = false;
    bool loopAnimation = false;
    float rotationSpeed = 0.0f;
    float currentRotation = 0.0f;
    
    RenderComponent(Type t, float w = 0.1f, float h = 0.1f) 
        : type(t), width(w), height(h), currentFrame(0), 
          animationTimer(0.0f), frameTime(0.05f), frameCount(8),
          frameWidth(33), loopAnimation(true) {
        frameRect = sf::IntRect(0, 0, 33, 36);
        sprite.setOrigin(16.5f, 18.0f);
    }
};

struct PlayerComponent : public Component {
    enum class AnimationState {
        IDLE,
        UP,
        DOWN
    };
    AnimationState state = AnimationState::IDLE;
    bool hasTripleShot = false;
    float tripleShotDuration = 0.0f;
    bool isLocalPlayer = false;
};

struct BulletComponent : public Component {
    float speed = 0.02f;
    float dy = 0.0f;

    BulletComponent(float _speed = 0.02f, float _dy = 0.0f) 
        : speed(_speed), dy(_dy) {}
};

struct EnemyComponent : public Component {
    enum class Type {
        BASIC,
        FAST,
        TANK,
        BOSS
    };
    
    Type type;
    float speed;
    float shootTimer = 0.0f;
    float shootInterval = 2.0f;
    
    EnemyComponent(Type t = Type::BASIC, float s = 0.005f) 
        : type(t), speed(s) {}
};

struct BossComponent : public Component {
    float speed;
    float direction;
    float shootTimer;
    float shootInterval;
    int health;
    bool isActive;
    float spawnTimer;
    
    BossComponent(float s = 0.003f) 
        : speed(s), direction(1.0f), shootTimer(0.0f), 
          shootInterval(1.0f), health(5), isActive(false), 
          spawnTimer(10.0f) {}
};

struct ScoreComponent : public Component {
    int score;
    explicit ScoreComponent(int initialScore = 0) : score(initialScore) {}
};

struct ExplosionComponent : public Component {
    float lifetime;
    float currentTime;
    
    ExplosionComponent() : lifetime(0.4f), currentTime(0.0f) {}
};

struct MovementPatternComponent : public Component {
    enum class Pattern {
        LINEAR,
        SINUSOIDAL,
        CIRCULAR
    };

    Pattern pattern;
    float amplitude;
    float frequency;
    float phase;

    MovementPatternComponent(Pattern p, float a = 1.0f, float f = 1.0f, float ph = 0.0f)
        : pattern(p), amplitude(a), frequency(f), phase(ph) {}
};

struct PowerUpComponent : public Component {
    enum class Type {
        TRIPLE_SHOT
    };
    Type type;
    PowerUpComponent(Type t = Type::TRIPLE_SHOT) : type(t) {}
};

class ObstacleComponent : public Component {
public:
    ObstacleComponent(float pushForce = 0.01f) : pushForce(pushForce) {}
    ~ObstacleComponent() override = default;

    void update() override {}

    float pushForce;
};