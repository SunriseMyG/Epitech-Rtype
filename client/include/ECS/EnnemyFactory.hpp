/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** EnnemyFactory
*/

#pragma once
#include <memory>
#include "./Component.hpp"
#include "Entity.hpp"
#include "TextureManager.hpp"
#include <cmath>

class EnemyFactory {
public:
    enum class EnemyType {
        BASIC,
        FAST,
        TANK,
        BOSS_TYPE_1,
        BOSS_TYPE_2,
        BOSS_TYPE_3,
    };

    static std::shared_ptr<Entity> createEnemy(std::string type, float x, float y) {
        auto enemy = std::make_shared<Entity>();
        enemy->addComponent<PositionComponent>(x, y);
        enemy->addComponent<RenderComponent>(RenderComponent::Type::ENEMY);

        EnemyType enemyType;
        if (type == "BASIC") {
            enemyType = EnemyType::BASIC;
        } else if (type == "FAST") {
            enemyType = EnemyType::FAST;
        } else if (type == "TANK") {
            enemyType = EnemyType::TANK;
        }

        auto render = RenderComponent(RenderComponent::Type::ENEMY);

        switch (enemyType) {
            case EnemyType::BASIC:
                std::cout << "Création de l'ennemi de type BASIC" << std::endl;
                enemy->addComponent<EnemyComponent>(EnemyComponent::Type::BASIC, 0.001f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet3.png");
                render.frameRect = sf::IntRect(0, 0, 17, 17);
                render.sprite.setScale(2.5f, 2.5f);
                enemy->addComponent<MovementPatternComponent>(MovementPatternComponent::Pattern::LINEAR);
                render.sprite.setOrigin(render.frameRect.width / 2.0f, render.frameRect.height / 2.0f);
                render.frameTime = 0.08f;
                render.frameCount = 8;
                break;

            case EnemyType::FAST:
                enemy->addComponent<EnemyComponent>(EnemyComponent::Type::FAST, 0.004f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet23.png");
                render.frameRect = sf::IntRect(0, 0, 33, 33);
                render.sprite.setScale(2.5f, 2.5f);
                enemy->addComponent<MovementPatternComponent>(
                    MovementPatternComponent::Pattern::SINUSOIDAL,
                    0.05f,
                    3.0f
                );
                render.sprite.setOrigin(16.5f, 16.5f);
                render.frameTime = 0.1f;
                render.frameCount = 8;
                break;

            case EnemyType::TANK:
                enemy->addComponent<EnemyComponent>(EnemyComponent::Type::TANK, 0.002f);
                render.texture = TextureManager::loadTexture("client/sprites/r-typesheet5.png");
                render.frameRect = sf::IntRect(0, 0, 33, 36);
                render.sprite.setScale(4.0f, 4.0f);
                enemy->addComponent<MovementPatternComponent>(MovementPatternComponent::Pattern::LINEAR);
                render.sprite.setOrigin(16.5f, 18.0f);
                render.frameTime = 0.1f;
                render.frameCount = 8;
                break;
        }

        render.currentFrame = 0;
        render.animationTimer = 0.0f;
        render.sprite.setTexture(*render.texture);
        render.sprite.setTextureRect(render.frameRect);
        render.sprite.setPosition(x, y);
        enemy->addComponent<RenderComponent>(render);
        return enemy;
    }
};
