/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** System
*/

#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include "Entity.hpp"
#include "EnnemyFactory.hpp"
#include "BossFactory.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include "World.hpp"
#include "../Menu/Menu.hpp"
#include "../Menu/MenuState.hpp"
#include "./SoundManager.hpp"
#include "./ObstacleFactory.hpp"
#include "Component.hpp"

extern sf::RenderWindow window;

class System {
public:
    virtual ~System() = default;
    virtual void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime = 0.0f) = 0;
};
class MovementSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*for (auto& entity : entities) {
            if (entity->hasComponent<PositionComponent>() && 
                entity->hasComponent<VelocityComponent>()) {
                auto pos = entity->template getComponent<PositionComponent>();
                auto vel = entity->getComponent<VelocityComponent>();
                
                float newX = pos->x + vel->dx;
                float newY = pos->y + vel->dy;
                
                if (newX >= -0.95f && newX <= 0.95f) {
                    pos->x = newX;
                }
                if (newY >= -0.95f && newY <= 0.95f) {
                    pos->y = newY;
                }
            }
        }*/
    }
};

class BulletSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*auto it = std::remove_if(entities.begin(), entities.end(),
            [](const auto& entity) {
                if (!entity->template hasComponent<BulletComponent>() || 
                    !entity->template hasComponent<PositionComponent>()) {
                    return false;
                }
                
                auto pos = entity->template getComponent<PositionComponent>();
                return pos->x > 1.2f || pos->x < -1.2f || 
                       pos->y > 1.2f || pos->y < -1.2f;
            });
        
        entities.erase(it, entities.end());

        for (auto& entity : entities) {
            if (entity->template hasComponent<BulletComponent>() && 
                entity->template hasComponent<PositionComponent>()) {
                auto pos = entity->template getComponent<PositionComponent>();
                auto bullet = entity->template getComponent<BulletComponent>();
                if (pos && bullet) {
                    pos->x += bullet->speed;
                    pos->y += bullet->dy;
                }
            }
        }*/
    }
};

class EnemySystem : public System {
public:

    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*if (!spawnEnabled) {
            return; 
        }*/

        //spawnTimer += 0.016f;

        /*if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            float randomY = (float)(rand() % 180 - 90) / 100.0f;
            
            if (rand() % 100 < 20) {
                auto obstacle = ObstacleFactory::createObstacle(1.0f, randomY);
                entities.push_back(obstacle);
            } else {
                EnemyFactory::EnemyType enemyType;
                if (basicSpawnCount >= 5) {
                    enemyType = EnemyFactory::EnemyType::TANK;
                    basicSpawnCount = 0;
                } else {
                    enemyType = EnemyFactory::EnemyType::BASIC;
                    basicSpawnCount++;
                }
                auto enemy = EnemyFactory::createEnemy(enemyType, 1.0f, randomY);
                entities.push_back(enemy);
            }
            spawnCounter++;
        }*/

        /*for (auto& entity : entities) {
            if (entity->hasComponent<EnemyComponent>() && 
                entity->hasComponent<PositionComponent>()) {
                auto pos = entity->getComponent<PositionComponent>();
                auto enemy = entity->getComponent<EnemyComponent>();
                pos->x -= enemy->speed;

                if (enemy->type == EnemyComponent::Type::BASIC) {
                    enemy->shootTimer += deltaTime;
                    if (enemy->shootTimer >= enemy->shootInterval) {
                        enemy->shootTimer = 0.0f;
                        
                        auto bullet = std::make_shared<Entity>();
                        bullet->addComponent<PositionComponent>(pos->x, pos->y);
                        bullet->addComponent<BulletComponent>(-0.015f);
                        
                        auto render = RenderComponent(RenderComponent::Type::BULLET);
                        render.texture = TextureManager::loadTexture("client/sprites/porjectile-ennemie.png");
                        render.sprite.setTexture(*render.texture);
                        render.frameRect = sf::IntRect(0, 0, 16, 16);
                        render.sprite.setTextureRect(render.frameRect);
                        render.sprite.setOrigin(8.0f, 8.0f);
                        render.sprite.setScale(2.0f, 2.0f);
                        render.rotationSpeed = 360.0f;
                        
                        bullet->addComponent<RenderComponent>(render);
                        entities.push_back(bullet);
                    }
                }
            }
        }*/
    }

private:
    float spawnTimer = 0.0f;
    float spawnInterval = 0.4f;
    int spawnCounter = 0;
    int basicSpawnCount = 0;
    const bool spawnEnabled = false;  // Nouvelle variable statique

};

class CollisionSystem : public System {
public:
    CollisionSystem(NetworkManager& networkManager) : networkManager(networkManager) {}

    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        
        // Supprimer les entités marquées pour suppression
        auto it = std::remove_if(entities.begin(), entities.end(),
            [](const auto& entity) {
                return entity && entity->isMarkedForDeletion();
            });
        entities.erase(it, entities.end());
 /*
        // Vérifier les collisions projectiles-ennemis et projectiles-boss
        for (const auto& bullet : entities) {
            if (!bullet || !bullet->hasComponent<BulletComponent>()) continue;
            
            auto bullet_pos = bullet->getComponent<PositionComponent>();
            auto bullet_comp = bullet->getComponent<BulletComponent>();
            
            if (!bullet_pos || !bullet_comp || bullet_comp->speed < 0) continue; // Ignorer les projectiles ennemis

            // Collisions avec les ennemis
            for (const auto& enemy : entities) {
                if (!enemy || !enemy->hasComponent<EnemyComponent>()) continue;
                
                auto enemy_pos = enemy->getComponent<PositionComponent>();
                if (!enemy_pos) continue;

                if (checkCollision(bullet_pos->x, bullet_pos->y, enemy_pos->x, enemy_pos->y)) {
                    // Vérifier que l'ennemi existe et a un ID valide
                    // if (enemy && enemy->hasId()) {
                    //     int enemyId = enemy->getId();
                        
                    //     // Envoyer le message au serveur avant de supprimer l'ennemi
                    //     std::string message = "111 " + std::to_string(enemyId);
                    //     networkManager.sendTCP(message);
                    // }
                    
                    // Marquer pour suppression
                    enemy->markForDelete();
                    bullet->markForDelete();
                    
                    // Mettre à jour le score du joueur
                    for (const auto& entity : entities) {
                        if (entity->hasComponent<PlayerComponent>() && 
                            entity->hasComponent<ScoreComponent>()) {
                            auto score = entity->getComponent<ScoreComponent>();
                            score->score += 100; // Augmenter le score de 100 points
                            break;
                        }
                    }
                    
                    // Créer une explosion
                    auto explosion = std::make_shared<Entity>();
                    explosion->addComponent<PositionComponent>(enemy_pos->x, enemy_pos->y);
                    explosion->addComponent<ExplosionComponent>();
                    
                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 32, 32);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.0f, 16.0f);
                    render.sprite.setScale(4.0f, 4.0f);
                    render.frameTime = 0.07f;
                    render.frameCount = 6;
                    render.frameWidth = 32;
                    render.loopAnimation = false;
                    render.currentFrame = 0;
                    render.animationTimer = 0.0f;
                    
                    explosion->addComponent<RenderComponent>(render);
                    entities.push_back(explosion);
                    
                    SoundManager::playSound("client/sound/explosion-91872.ogg");
                    break;
                }
            }

            // Collisions avec le boss
            for (const auto& boss : entities) {
                if (!boss || !boss->hasComponent<BossComponent>()) continue;
                
                auto boss_pos = boss->getComponent<PositionComponent>();
                auto boss_comp = boss->getComponent<BossComponent>();
                if (!boss_pos || !boss_comp || !boss_comp->isActive) continue;

                if (checkCollision(bullet_pos->x, bullet_pos->y, boss_pos->x, boss_pos->y)) {
                    bullet->markForDelete();
                    boss_comp->health--;
                    
                    if (boss_comp->health <= 0) {
                        boss->markForDelete();
                        
                        // Créer une explosion pour le boss
                        auto explosion = std::make_shared<Entity>();
                        explosion->addComponent<PositionComponent>(boss_pos->x, boss_pos->y);
                        explosion->addComponent<ExplosionComponent>();
                        
                        auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                        render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                        render.sprite.setTexture(*render.texture);
                        render.frameRect = sf::IntRect(0, 0, 32, 32);
                        render.sprite.setTextureRect(render.frameRect);
                        render.sprite.setOrigin(16.0f, 16.0f);
                        render.sprite.setScale(6.0f, 6.0f);
                        render.frameTime = 0.07f;
                        render.frameCount = 6;
                        render.frameWidth = 32;
                        render.loopAnimation = false;
                        render.currentFrame = 0;
                        render.animationTimer = 0.0f;
                        
                        explosion->addComponent<RenderComponent>(render);
                        entities.push_back(explosion);
                        
                        SoundManager::playSound("client/sound/explosion-91872.ogg");
                    }
                    break;
                }
            }
        }*/

        // Vérifier les collisions projectiles-obstacles
        /*for (const auto& bullet : entities) {
            if (!bullet || !bullet->hasComponent<BulletComponent>()) continue;
            
            auto bullet_pos = bullet->getComponent<PositionComponent>();
            if (!bullet_pos) continue;

            // Collisions avec les obstacles
            for (const auto& obstacle : entities) {
                if (!obstacle || !obstacle->hasComponent<ObstacleComponent>()) continue;
                
                auto obstacle_pos = obstacle->getComponent<PositionComponent>();
                if (!obstacle_pos) continue;

                if (checkCollision(bullet_pos->x, bullet_pos->y, obstacle_pos->x, obstacle_pos->y)) {
                    bullet->markForDelete();
                    
                    // Créer une petite explosion
                    auto explosion = std::make_shared<Entity>();
                    explosion->addComponent<PositionComponent>(bullet_pos->x, bullet_pos->y);
                    explosion->addComponent<ExplosionComponent>();
                    
                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 32, 32);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.0f, 16.0f);
                    render.sprite.setScale(2.0f, 2.0f);  // Plus petite explosion que pour les ennemis
                    render.frameTime = 0.07f;
                    render.frameCount = 6;
                    render.frameWidth = 32;
                    render.loopAnimation = false;
                    render.currentFrame = 0;
                    render.animationTimer = 0.0f;
                    
                    explosion->addComponent<RenderComponent>(render);
                    entities.push_back(explosion);
                    break;
                }
            }
        }*/

        // Garder la logique existante pour les collisions joueur-ennemis
        /*for (const auto& player : entities) {
            if (!player || !player->hasComponent<PlayerComponent>()) continue;
            
            auto player_pos = player->getComponent<PositionComponent>();
            if (!player_pos) continue;

            // Vérification des collisions avec les ennemis
            for (const auto& enemy : entities) {
                if (!enemy || !enemy->hasComponent<EnemyComponent>()) continue;
                
                auto enemy_pos = enemy->getComponent<PositionComponent>();
                if (!enemy_pos) continue;

                if (checkCollision(player_pos->x, player_pos->y, enemy_pos->x, enemy_pos->y)) {
                    // Marquer le joueur pour suppression
                    player->markForDelete();

                    // Jouer le son d'explosion
                    SoundManager::playSound("client/sound/explosion-91872.ogg");

                    // Créer une explosion
                    auto explosion = std::make_shared<Entity>();
                    explosion->addComponent<PositionComponent>(player_pos->x, player_pos->y);
                    explosion->addComponent<ExplosionComponent>();
                    
                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 32, 32);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.0f, 16.0f);
                    render.sprite.setScale(4.0f, 4.0f);
                    render.frameTime = 0.07f;
                    render.frameCount = 6;
                    render.frameWidth = 32;
                    render.loopAnimation = false;
                    render.currentFrame = 0;
                    render.animationTimer = 0.0f;
                    
                    explosion->addComponent<RenderComponent>(render);
                    entities.push_back(explosion);

                    // Passer à l'écran game over
                    Menu::setGlobalState(MenuState::GAME_OVER);
                    break;
                }
            }
        }*/

        // Vérification des collisions joueur-powerup
        /*for (const auto& powerUp : entities) {
            if (!powerUp || !powerUp->hasComponent<PowerUpComponent>()) continue;
            
            auto powerUp_pos = powerUp->getComponent<PositionComponent>();
            if (!powerUp_pos) continue;

            for (const auto& player : entities) {
                if (!player || !player->hasComponent<PlayerComponent>()) continue;
                
                auto player_pos = player->getComponent<PositionComponent>();
                if (!player_pos) continue;

                if (checkCollision(player_pos->x, player_pos->y, powerUp_pos->x, powerUp_pos->y)) {
                    auto playerComp = player->getComponent<PlayerComponent>();
                    playerComp->hasTripleShot = true;
                    playerComp->tripleShotDuration = 10.0f;
                    powerUp->markForDelete();
                    SoundManager::playSound("client/sound/powerup.ogg");
                    break;
                }
            }
        }*/

        // Vérification des collisions joueur-boss
        /*for (const auto& player : entities) {
            if (!player || !player->hasComponent<PlayerComponent>()) continue;
            
            auto player_pos = player->getComponent<PositionComponent>();
            if (!player_pos) continue;

            // Collision avec le boss
            for (const auto& boss : entities) {
                if (!boss || !boss->hasComponent<BossComponent>()) continue;
                
                auto boss_pos = boss->getComponent<PositionComponent>();
                auto boss_comp = boss->getComponent<BossComponent>();
                if (!boss_pos || !boss_comp || !boss_comp->isActive) continue;

                if (checkCollision(player_pos->x, player_pos->y, boss_pos->x, boss_pos->y)) {
                    // Marquer le joueur pour suppression
                    player->markForDelete();

                    // Jouer le son d'explosion
                    SoundManager::playSound("client/sound/explosion-91872.ogg");

                    // Créer une explosion
                    auto explosion = std::make_shared<Entity>();
                    explosion->addComponent<PositionComponent>(player_pos->x, player_pos->y);
                    explosion->addComponent<ExplosionComponent>();
                    
                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 32, 32);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.0f, 16.0f);
                    render.sprite.setScale(4.0f, 4.0f);
                    render.frameTime = 0.07f;
                    render.frameCount = 6;
                    render.frameWidth = 32;
                    render.loopAnimation = false;
                    render.currentFrame = 0;
                    render.animationTimer = 0.0f;
                    
                    explosion->addComponent<RenderComponent>(render);
                    entities.push_back(explosion);

                    // Passer à l'écran game over
                    Menu::setGlobalState(MenuState::GAME_OVER);
                    break;
                }
            }
        }*/

        // Après la vérification des collisions joueur-ennemis existante
        // Ajoutons la vérification des collisions joueur-projectiles du boss
        /*for (const auto& player : entities) {
            if (!player || !player->hasComponent<PlayerComponent>()) continue;
            
            auto player_pos = player->getComponent<PositionComponent>();
            if (!player_pos) continue;

            // Vérification des collisions avec les projectiles
            for (const auto& bullet : entities) {
                if (!bullet || !bullet->hasComponent<BulletComponent>()) continue;
                
                auto bullet_pos = bullet->getComponent<PositionComponent>();
                auto bullet_comp = bullet->getComponent<BulletComponent>();
                
                if (!bullet_pos || !bullet_comp || bullet_comp->speed >= 0) continue; // Uniquement les projectiles ennemis

                if (checkCollision(player_pos->x, player_pos->y, bullet_pos->x, bullet_pos->y)) {
                    // Marquer le joueur et le projectile pour suppression
                    player->markForDelete();
                    bullet->markForDelete();

                    // Créer une explosion
                    auto explosion = std::make_shared<Entity>();
                    explosion->addComponent<PositionComponent>(player_pos->x, player_pos->y);
                    explosion->addComponent<ExplosionComponent>();
                    
                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 32, 32);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.0f, 16.0f);
                    render.sprite.setScale(4.0f, 4.0f);
                    render.frameTime = 0.07f;
                    render.frameCount = 6;
                    render.frameWidth = 32;
                    render.loopAnimation = false;
                    render.currentFrame = 0;
                    render.animationTimer = 0.0f;
                    
                    explosion->addComponent<RenderComponent>(render);
                    entities.push_back(explosion);

                    // Jouer le son d'explosion
                    SoundManager::playSound("client/sound/explosion-91872.ogg");

                    // Passer à l'écran game over
                    Menu::setGlobalState(MenuState::GAME_OVER);
                    break;
                }
            }
        }*/

        // Garder la logique existante pour les autres collisions
        // [Le reste du code existant...]
    }

private:
    bool checkCollision(float x1, float y1, float x2, float y2) {
        const float threshold = 0.05f;
        return std::abs(x1 - x2) < threshold && std::abs(y1 - y2) < threshold;
    }

    NetworkManager& networkManager;
};

class RenderSystem : public System {
private:
    sf::RenderWindow& window;

public:
    RenderSystem(sf::RenderWindow& window) : window(window) {}
    
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        for (const auto& entity : entities) {
            auto render = entity->getComponent<RenderComponent>();
            auto pos = entity->getComponent<PositionComponent>();
            
            if (!render || !pos) {
                std::cout << "Entité sans composants de rendu ou de position" << std::endl;
                continue;
            }
            
            // Gestion de l'animation ici
            if (render->frameCount > 1) {
                render->animationTimer += deltaTime;
                if (render->animationTimer >= render->frameTime) {
                    render->animationTimer = 0;
                    render->currentFrame = (render->currentFrame + 1) % render->frameCount;
                    if (entity->hasComponent<EnemyComponent>()) {
                        if (entity->getComponent<EnemyComponent>()->type == EnemyComponent::Type::TANK) {
                            render->frameRect.left = render->currentFrame * 33;
                        } else if (entity->getComponent<EnemyComponent>()->type == EnemyComponent::Type::FAST) {
                            render->frameRect.left = render->currentFrame * 33;
                        } else {
                            render->frameRect.left = render->currentFrame * 17;
                        }
                    } else if (entity->hasComponent<PowerUpComponent>()) {
                        render->frameRect.left = render->currentFrame * 34;
                    }
                    render->sprite.setTextureRect(render->frameRect);
                }
            }
            
            float screenX = (pos->x + 1.0f) * window.getSize().x / 2.0f;
            float screenY = (pos->y + 1.0f) * window.getSize().y / 2.0f;
            
            render->sprite.setPosition(screenX, screenY);
            window.draw(render->sprite);
        }
    }
};

class BossSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*bool bossExists = false;
        
        for (auto& entity : entities) {
            if (entity->hasComponent<BossComponent>()) {
                bossExists = true;
                auto boss = entity->getComponent<BossComponent>();
                auto pos = entity->getComponent<PositionComponent>();
                
                if (!boss->isActive) {
                    boss->spawnTimer -= 0.016f;
                    if (boss->spawnTimer <= 0) {
                        boss->isActive = true;
                    }
                    continue;
                }

                // Entrée en scène
                if (pos->x > 0.8f) {
                    pos->x -= 0.01f;
                    continue;
                }

                // Mouvement vertical
                pos->y += boss->speed * boss->direction;
                if (pos->y >= 0.95f) {
                    boss->direction = -1.0f;
                } else if (pos->y <= -0.95f) {
                    boss->direction = 1.0f;
                }

                // Tir
                boss->shootTimer += 0.016f;
                if (boss->shootTimer >= boss->shootInterval) {
                    boss->shootTimer = 0.0f;
                    auto bullet = std::make_shared<Entity>();
                    bullet->addComponent<PositionComponent>(pos->x - 0.1f, pos->y);
                    bullet->addComponent<BulletComponent>(-0.02f);

                    auto render = RenderComponent(RenderComponent::Type::BULLET);
                    render.texture = TextureManager::loadTexture("client/sprites/r-type-projectile.png");
                    render.sprite.setTexture(*render.texture);
                    render.frameRect = sf::IntRect(0, 0, 33, 36);
                    render.sprite.setTextureRect(render.frameRect);
                    render.sprite.setOrigin(16.5f, 18.0f);
                    render.sprite.setScale(-1.0f, 1.0f);  // Scale négatif pour inverser le sprite
                    render.frameTime = 0.1f;
                    render.frameCount = 2;

                    bullet->addComponent<RenderComponent>(render);
                    entities.push_back(bullet);
                }
            }
        }*/

        // if (!bossExists) {
        //     auto boss = BossFactory::createBoss();
        //     entities.push_back(boss);
        // }
    }
};

class ScoreRenderSystem : public System {
public:
    ScoreRenderSystem(sf::RenderWindow& window) : window(window) {
        if (!font.loadFromFile("client/font/Vipnagorgialla.otf")) {
            throw std::runtime_error("Failed to load font");
        }
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(10, 10);
    }

    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        for (const auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>() && 
                entity->hasComponent<ScoreComponent>()) {
                auto score = entity->getComponent<ScoreComponent>();
                scoreText.setString("Score: " + std::to_string(score->score));
                window.draw(scoreText);
                break;
            }
        }
    }

private:
    sf::RenderWindow& window;
    sf::Font font;
    sf::Text scoreText;
};

class ExplosionSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        auto it = std::remove_if(entities.begin(), entities.end(),
            [](const std::shared_ptr<Entity>& entity) {
                auto explosion = entity->getComponent<ExplosionComponent>();
                if (!explosion) return false;
                
                explosion->currentTime += 0.016f;
                return explosion->currentTime >= explosion->lifetime;
            });
        entities.erase(it, entities.end());
    }
};

class MovementPatternSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*for (auto& entity : entities) {
            auto position = entity->getComponent<PositionComponent>();
            auto pattern = entity->getComponent<MovementPatternComponent>();
            auto enemy = entity->getComponent<EnemyComponent>();

            if (position && pattern && enemy) {
                float baseSpeed = enemy->speed;
                float time = static_cast<float>(clock.getElapsedTime().asSeconds());

                switch (pattern->pattern) {
                    case MovementPatternComponent::Pattern::LINEAR:
                        position->x -= baseSpeed;
                        break;
                    case MovementPatternComponent::Pattern::SINUSOIDAL:
                        position->x -= baseSpeed;
                        position->y += std::sin(time * pattern->frequency + pattern->phase) * pattern->amplitude;
                        break;
                    case MovementPatternComponent::Pattern::CIRCULAR:
                        position->x -= baseSpeed;
                        position->y += std::sin(time * pattern->frequency + pattern->phase) * pattern->amplitude;
                        position->x += std::cos(time * pattern->frequency + pattern->phase) * pattern->amplitude;
                        break;
                }
            }
        }*/
    }

private:
    sf::Clock clock;
};

class PowerUpSystem : public System {
private:
    float spawnTimer = 0.0f;
    float spawnInterval = 1.0f;
    bool initialSpawn = false;
    const float POWERUP_SPEED = 0.005f;  // Vitesse de déplacement du bonus
    bool spawnEnabled = false;

public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*// Spawn initial du bonus

        if (!spawnEnabled) {
            return;
        }

        if (!initialSpawn) {
            initialSpawn = true;
            spawnPowerUp(entities, 0.0f, 0.0f);
        }

        // Déplacement des power-ups
        for (auto& entity : entities) {
            if (entity->hasComponent<PowerUpComponent>() && 
                entity->hasComponent<PositionComponent>()) {
                auto pos = entity->getComponent<PositionComponent>();
                pos->x -= POWERUP_SPEED;  // Déplacement vers la gauche

                // Suppression si hors écran
                if (pos->x < -1.0f) {
                    entity->markForDelete();
                }
            }
        }

        // Spawn périodique
        spawnTimer += deltaTime;
        if (spawnTimer >= spawnInterval) {
            spawnTimer = 0.0f;
            if (rand() % 100 < 20) {
                float randomY = (float)(rand() % 180 - 90) / 100.0f;
                spawnPowerUp(entities, 1.0f, randomY);
            }
        }

        // Mise à jour de la durée du triple tir
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>()) {
                auto player = entity->getComponent<PlayerComponent>();
                if (player->hasTripleShot) {
                    player->tripleShotDuration -= deltaTime;
                    if (player->tripleShotDuration <= 0) {
                        player->hasTripleShot = false;
                    }
                }
            }
        }*/
    }

private:
    void spawnPowerUp(std::vector<std::shared_ptr<Entity>>& entities, float x, float y) {
        /*auto powerUp = std::make_shared<Entity>();
        powerUp->addComponent<PositionComponent>(x, y);
        powerUp->addComponent<PowerUpComponent>();
        
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
        
        powerUp->addComponent<RenderComponent>(render);
        entities.push_back(powerUp);*/
    }
};

class ObstacleSystem : public System {
public:
    void update(std::vector<std::shared_ptr<Entity>>& entities, float deltaTime) override {
        /*for (auto& obstacle : entities) {
            if (!obstacle || !obstacle->hasComponent<ObstacleComponent>()) continue;
            
            auto pos = obstacle->getComponent<PositionComponent>();
            if (!pos) continue;
            
            // Déplacer l'obstacle vers la gauche
            pos->x -= 0.005f;
            
            // Vérifier les collisions avec le joueur
            for (auto& player : entities) {
                if (!player || !player->hasComponent<PlayerComponent>()) continue;
                
                auto playerPos = player->getComponent<PositionComponent>();
                if (!playerPos) continue;
                
                if (checkCollision(pos->x, pos->y, playerPos->x, playerPos->y)) {
                    // Calculer la direction de la collision
                    float dx = playerPos->x - pos->x;
                    float dy = playerPos->y - pos->y;
                    
                    // Déterminer quelle face a été touchée
                    if (std::abs(dx) > std::abs(dy)) {
                        // Collision horizontale
                        if (dx > 0) {
                            playerPos->x = pos->x + 0.1f;
                        } else {
                            playerPos->x = pos->x - 0.1f;
                        }
                    } else {
                        if (dy > 0) {
                            playerPos->y = pos->y + 0.1f;
                        } else {
                            playerPos->y = pos->y - 0.1f;
                        }
                    }

                    // Vérifier si le joueur sort de l'écran à gauche
                    if (playerPos->x < -1.0f) {
                        // Marquer le joueur pour suppression
                        player->markForDelete();

                        // Créer une explosion
                        auto explosion = std::make_shared<Entity>();
                        explosion->addComponent<PositionComponent>(playerPos->x, playerPos->y);
                        explosion->addComponent<ExplosionComponent>();
                        
                        auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                        render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png");
                        render.sprite.setTexture(*render.texture);
                        render.frameRect = sf::IntRect(0, 0, 32, 32);
                        render.sprite.setTextureRect(render.frameRect);
                        render.sprite.setOrigin(16.0f, 16.0f);
                        render.sprite.setScale(4.0f, 4.0f);
                        render.frameTime = 0.07f;
                        render.frameCount = 6;
                        render.frameWidth = 32;
                        render.loopAnimation = false;
                        render.currentFrame = 0;
                        render.animationTimer = 0.0f;
                        
                        explosion->addComponent<RenderComponent>(render);
                        entities.push_back(explosion);

                        // Jouer le son d'explosion
                        SoundManager::playSound("client/sound/explosion-91872.ogg");

                        // Passer à l'écran game over
                        Menu::setGlobalState(MenuState::GAME_OVER);
                    }
                }
            }
            
            // Supprimer l'obstacle s'il sort de l'écran
            if (pos->x < -1.2f) {
                obstacle->markForDelete();
            }
        }*/
    }

private:
    bool checkCollision(float x1, float y1, float x2, float y2) {
        const float threshold = 0.1f;
        return std::abs(x1 - x2) < threshold && std::abs(y1 - y2) < threshold;
    }
};