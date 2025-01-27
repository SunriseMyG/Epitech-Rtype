#pragma once
#include <memory>
#include "./Component.hpp"
#include "Entity.hpp"

class BossFactory {
public:
    enum class BossType {
        STAGE1,
        STAGE2,
        STAGE3,
        STAGE4,
        STAGE5
    };

    static std::shared_ptr<Entity> createBoss(BossType type) {
        auto boss = std::make_shared<Entity>();
        boss->addComponent<PositionComponent>(1.2f, 0.0f);
        boss->addComponent<VelocityComponent>();
        boss->addComponent<BossComponent>();
        
        auto render = RenderComponent(RenderComponent::Type::BOSS);

        switch (type) {
            case BossType::STAGE1:
                boss->addComponent<BossComponent>(0.002f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet26.png");
                render.frameWidth = 65;
                render.frameHeight = 64;
                render.frameRect = sf::IntRect(32, 0, render.frameWidth, render.frameHeight);
                render.sprite.setScale(3.0f, 3.0f);
                render.sprite.setOrigin(render.frameWidth / 2.0f, render.frameHeight / 2.0f);
                render.frameTime = 0.25f;
                render.frameCount = 3;
                break;

            case BossType::STAGE2:
                boss->addComponent<BossComponent>(0.0025f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet30.png");
                render.frameRect = sf::IntRect(0, 0, 96, 96);
                render.sprite.setScale(3.5f, 3.5f);
                render.sprite.setOrigin(48.0f, 48.0f);
                render.frameTime = 0.18f;
                render.frameCount = 4;
                break;

            case BossType::STAGE3:
                boss->addComponent<BossComponent>(0.003f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet33.png");
                render.frameRect = sf::IntRect(0, 0, 128, 128);
                render.sprite.setScale(4.0f, 4.0f);
                render.sprite.setOrigin(64.0f, 64.0f);
                render.frameTime = 0.15f;
                render.frameCount = 5;
                break;

            case BossType::STAGE4:
                boss->addComponent<BossComponent>(0.0035f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet37.png");
                render.frameRect = sf::IntRect(0, 0, 160, 160);
                render.sprite.setScale(4.5f, 4.5f);
                render.sprite.setOrigin(80.0f, 80.0f);
                render.frameTime = 0.12f;
                render.frameCount = 6;
                break;

            case BossType::STAGE5:
                boss->addComponent<BossComponent>(0.004f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet42.png");
                render.frameRect = sf::IntRect(0, 0, 192, 192);
                render.sprite.setScale(5.0f, 5.0f);
                render.sprite.setOrigin(96.0f, 96.0f);
                render.frameTime = 0.1f;
                render.frameCount = 8;
                break;
        }

        render.sprite.setTexture(*render.texture);
        render.sprite.setTextureRect(render.frameRect);
        render.currentFrame = 0;
        render.animationTimer = 0.0f;

        boss->addComponent<RenderComponent>(render);
        return boss;
    }
};