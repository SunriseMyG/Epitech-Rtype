/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** EntityFactory
*/

#include "../include/EntityFactory.hpp"


void EntityFactory::createPlayer(World& world, float x, float y, int playerId, int netId) {
    auto player = std::make_shared<Entity>();
    
    player->addComponent<PositionComponent>(x, y);
    player->addComponent<VelocityComponent>();
    player->addComponent<PlayerComponent>();
    player->addComponent<ScoreComponent>(0);
    
    auto render = RenderComponent(RenderComponent::Type::PLAYER);
    render.texture = TextureManager::loadTexture("client/sprites/r-typesheet42.png");
    render.sprite.setTexture(*render.texture);
    int random = rand() % 5;
    render.frameRect = sf::IntRect(0, 0 + random * 18.0f - 1, 33, 18);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(4.0f, 4.0f);
    render.sprite.setPosition(x, y);
    
    player->addComponent<RenderComponent>(render);
    player->setId(playerId);
    player->setNetId(netId);
    world.addEntity(player);
}

void EntityFactory::createBullet(World& world, float x, float y) {
    auto player = world.findEntityWithComponent<PlayerComponent>();
    if (!player) {
        createSingleBullet(world, x, y, 0.0f);
        return;
    }

    auto playerComp = player->getComponent<PlayerComponent>();
    if (!playerComp) {
        createSingleBullet(world, x, y, 0.0f);
        return;
    }
    
    if (playerComp->hasTripleShot) {
        createSingleBullet(world, x, y, 0.0f);
        createSingleBullet(world, x, y, 0.02f);
        createSingleBullet(world, x, y, -0.02f);
    } else {
        createSingleBullet(world, x, y, 0.0f);
    }
}

void EntityFactory::createSingleBullet(World& world, float x, float y, float angleOffset) {
    auto bullet = std::make_shared<Entity>();
    bullet->addComponent<PositionComponent>(x, y);
    bullet->addComponent<BulletComponent>(0.02f, angleOffset);
    
    auto render = RenderComponent(RenderComponent::Type::BULLET);
    render.texture = TextureManager::loadTexture("client/sprites/r-type-projectile.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 33, 36);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(1.0f, 1.0f);
    render.frameTime = 0.1f;
    render.frameCount = 3;
    
    float angle = (angleOffset / 0.02f) * 15.0f;
    render.sprite.setRotation(angle);
    
    bullet->addComponent<RenderComponent>(render);
    world.addEntity(bullet);
}

std::shared_ptr<Entity> EntityFactory::createEnemyBullet(World& world, float x, float y) {
    auto bullet = std::make_shared<Entity>();
    bullet->addComponent<PositionComponent>(x, y);
    bullet->addComponent<BulletComponent>(-0.02f);

    auto render = RenderComponent(RenderComponent::Type::BULLET);
    render.texture = TextureManager::loadTexture("client/sprites/porjectile-ennemie.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 33, 36);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.5f, 18.0f);
    render.sprite.setScale(-1.0f, 1.0f);
    bullet->addComponent<RenderComponent>(render);

    return bullet;
}

void EntityFactory::createPowerup(World& world, float x, float y, int id) {
    auto powerup = std::make_shared<Entity>();
    powerup->addComponent<PositionComponent>(x, y);
    powerup->addComponent<PowerUpComponent>();
    
    auto render = RenderComponent(RenderComponent::Type::POWERUP);
    render.texture = TextureManager::loadTexture("client/sprites/r-typesheet30a.png");
    render.sprite.setTexture(*render.texture);
    render.frameRect = sf::IntRect(0, 0, 34, 32);
    render.sprite.setTextureRect(render.frameRect);
    render.sprite.setOrigin(16.0f, 16.0f);
    render.sprite.setScale(2.0f, 2.0f);
    render.frameTime = 0.1f;
    render.frameCount = 3;
    render.frameWidth = 34;
    render.currentFrame = 0;
    render.animationTimer = 0.0f;
    render.loopAnimation = true;
    
    powerup->addComponent<RenderComponent>(render);
    powerup->setId(id);
    world.addEntity(powerup);
}