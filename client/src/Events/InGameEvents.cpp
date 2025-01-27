/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Events
*/
#include "../../include/Menu/Menu.hpp"
#include "../../include/ECS/Entity.hpp"
#include "../../include/EntityFactory.hpp"
#include <iostream>
#include <algorithm>
#include <thread>

void Menu::handleWinEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseButtonPressed) {
        if (backToRoomsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (world) {
                auto& entities = world->getEntities();
                for (auto& entity : entities) {
                    if (entity) {
                        entity->markForDelete();
                    }
                }
            }
            setState(MenuState::ROOM_SELECT);
            if (networkManager.isInRoom()) {
                networkManager.leaveRoom(currentRoomId);
            }
        }
    }
}

void Menu::handleGameOverEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseMoved) {
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (restartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            setState(MenuState::GAME);
        } else if (mainMenuFromGameOverButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            setState(MenuState::MAIN_MENU);
            if (networkManager.isInRoom()) {
                networkManager.leaveRoom(currentRoomId);
            }
        }
    }
}

void Menu::handleInGameKeyboardEvents() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && window.hasFocus()) {
        auto entities = world->getEntities();
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>()) {
                auto pos = entity->getComponent<PositionComponent>();
                if (!pos) continue;
                int playerId = networkManager.getPlayerId();
                float& shootCooldown = playerShootCooldowns[playerId];

                if (shootCooldown <= 0.0f) {
                    networkManager.sendUDP("107 SPACE");
                    
                    shootCooldown = 0.5f;
                }
                shootCooldown -= 0.016f;
                break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(keyBindings.moveUp) && window.hasFocus()) {
        auto entities = world->getEntities();
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>() ) {
                networkManager.sendUDP("107 UP");
                //auto pos = entity->getComponent<PositionComponent>();
                break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(keyBindings.moveLeft) && window.hasFocus()) {
        auto entities = world->getEntities();
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>()) {
                networkManager.sendUDP("107 LEFT");
                //auto pos = entity->getComponent<PositionComponent>();
                break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(keyBindings.moveDown) && window.hasFocus()) {
        auto entities = world->getEntities();
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>()) {
                networkManager.sendUDP("107 DOWN");
                //auto pos = entity->getComponent<PositionComponent>();
                break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(keyBindings.moveRight) && window.hasFocus()) {
        auto entities = world->getEntities();
        for (auto& entity : entities) {
            if (entity->hasComponent<PlayerComponent>()) {
                networkManager.sendUDP("107 RIGHT");
                //auto pos = entity->getComponent<PositionComponent>();
                break;
            }
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && window.hasFocus()) {
        networkManager.sendTCP("112");
        if (networkManager.isInRoom()) {
            networkManager.leaveRoom(currentRoomId);
        }
        networkManager.requestLeaderboard();
        networkManager.requestRooms();
        setState(MenuState::ROOM_SELECT);
    }
}

void Menu::handleChangeSpritePlayer() {
    while (networkManager.hasActionMessages()) {
        std::string message = networkManager.getNextActionMessage();
        std::string protocol_id = message.substr(1, 3);
        std::string content = message.substr(5);
        
        if (protocol_id == "107") {
            std::istringstream iss(content);
            std::string status, playerId, keypressed;
            float x, y;
            
            if (iss >> status >> keypressed >> playerId >> x >> y) {
                for (auto& entity : world->getEntities()) {
                    if (entity->hasComponent<PlayerComponent>() && entity->getNetId() == std::stoi(playerId)) {
                        auto render = entity->getComponent<RenderComponent>();
                        if (!render) continue;
                        if (keypressed == "DOWN") {
                            render->frameRect.left = 0;
                        } else if (keypressed == "UP") {
                            render->frameRect.left = 99;
                        } else if (keypressed == "LEFT") {
                            render->frameRect.left = 33;
                        } else if (keypressed == "RIGHT") {
                            render->frameRect.left = 33;
                        }
                        render->sprite.setTextureRect(render->frameRect);
                    }
                }
            }
        }
    }
}


void Menu::handleGameEvents() {
    while (networkManager.hasGameStateMessages()) {
        std::string message = networkManager.getNextGameStateMessage();
        std::string protocol_id = message.substr(1, 3);
        std::string content = message.substr(5);

        if (protocol_id == "115") {
            try {
                JsonParser parser(content);
                json j = json::parse(content);
                
                for (const auto& [id, entity] : j.items()) {
                    std::string state = entity["state"].get<std::string>();

                    if (state == "CREATE") {
                        std::string group = entity["group"].get<std::string>();
                        if (group == "PLAYER") {
                            float y = entity["x"].get<float>();
                            float x = entity["y"].get<float>();
                            int netId = entity["ID NETWORK"].get<int>();
                            float normalizedX = (x / 50.0f) - 1.0f;
                            float normalizedY = (y / 50.0f) - 1.0f;
                            auto existingPlayer = world->findEntityById(std::stoi(id));
                            if (!existingPlayer) {
                                EntityFactory::createPlayer(*world, normalizedY, normalizedX, std::stoi(id), netId);
                            } else {
                                auto pos = existingPlayer->getComponent<PositionComponent>();
                                if (pos) {
                                    pos->x = normalizedY;
                                    pos->y = normalizedX;
                                }
                            }
                        } if (group == "ENEMY") {
                            
                            auto existingEnemy = world->findEntityById(std::stoi(id));
                            
                            float normalizedX = (entity["x"].get<float>() / 50.0f) - 1.0f;
                            float normalizedY = (entity["y"].get<float>() / 50.0f) - 1.0f;
                            std::string type = entity["type"].get<std::string>();



                            if (!existingEnemy) {
                                auto enemy = EnemyFactory::createEnemy(type, 1, normalizedX);
                                if (enemy) {
                                    enemy->setId(std::stoi(id));
                                    world->addEntity(enemy);
                                }
                            } else {
                                auto pos = existingEnemy->getComponent<PositionComponent>();
                                if (pos) {
                                    pos->x = 1;
                                    pos->y = normalizedX;
                                }
                            }
                        } else if (group == "ENEMY-BULLET") {
                            float normalizedX = (entity["x"].get<float>() / 50.0f) - 1.0f;
                            float normalizedY = (entity["y"].get<float>() / 50.0f) - 1.0f;
                            
                            auto existingBullet = world->findEntityById(std::stoi(id));
                            
                            if (!existingBullet) {
                                auto bullet = EntityFactory::createEnemyBullet(*world, normalizedX, normalizedY);
                                if (bullet) {
                                    bullet->setId(std::stoi(id));
                                    world->addEntity(bullet);
                                }
                            } else {
                                auto pos = existingBullet->getComponent<PositionComponent>();
                                if (pos) {
                                    pos->x = normalizedX;
                                    pos->y = normalizedY;
                                }
                            }
                        } else if (group == "PLAYER-BULLET") {
                            float normalizedX = (entity["x"].get<float>() / 50.0f) - 1.0f;
                            float normalizedY = (entity["y"].get<float>() / 50.0f) - 1.0f;
                            
                            auto existingBullet = world->findEntityById(std::stoi(id));
                            
                            if (!existingBullet) {
                                auto bullet = EntityFactory::createEnemyBullet(*world, normalizedX, normalizedY);
                                if (bullet) {
                                    bullet->setId(std::stoi(id));
                                    world->addEntity(bullet);
                                }
                            } else {
                                auto pos = existingBullet->getComponent<PositionComponent>();
                                if (pos) {
                                    pos->x = normalizedX;
                                    pos->y = normalizedY;
                                }
                            }
                        } else if (group == "POWERUP") {
                            float normalizedX = (entity["x"].get<float>() / 50.0f) - 1.0f;
                            float normalizedY = (entity["y"].get<float>() / 50.0f) - 1.0f;
                            std::string type = entity["type"].get<std::string>(); 
                            EntityFactory::createPowerup(*world, normalizedX, normalizedY, std::stoi(id));
                        }
                    } else if (state == "UPDATE") {
                        try {
                            JsonParser posParser(content);
                            auto positions = posParser.parseEntityPositions();
                            
                            for (const auto& pos : positions) {
                                auto entity = world->findEntityById(pos.id);
                                if (entity && entity->hasComponent<PositionComponent>()) {
                                    auto posComp = entity->getComponent<PositionComponent>();
                                    if (posComp) {
                                        float normalizedX = (pos.x / 50.0f) - 1.0f;
                                        float normalizedY = (pos.y / 50.0f) - 1.0f;
                                        
                                        posComp->x = normalizedX;
                                        posComp->y = normalizedY;
                                    }
                                    
                                    if (entity->hasComponent<ScoreComponent>() && pos.score >= 0) {
                                        auto scoreComp = entity->getComponent<ScoreComponent>();
                                        if (scoreComp) {
                                            scoreComp->score = pos.score;
                                        }
                                    }
                                }
                            }
                        } catch (const std::exception& e) {
                            continue;
                        }
                    } else if (state == "DELETE") {
                        auto existingEntity = world->findEntityById(std::stoi(id));
                        std::cout << "Delete entity: " << content << std::endl;
                        if (existingEntity && existingEntity->getId() != -1) {
                            if (existingEntity->hasComponent<EnemyComponent>()) {
                                auto pos = existingEntity->getComponent<PositionComponent>();
                                if (pos) {
                                    auto explosion = std::make_shared<Entity>();
                                    explosion->addComponent<PositionComponent>(pos->x, pos->y);
                                    explosion->addComponent<ExplosionComponent>();
                                    
                                    auto render = RenderComponent(RenderComponent::Type::EXPLOSION);
                                    render.texture = TextureManager::loadTexture("client/sprites/r-typeexplosion.png"); 
                                    render.sprite.setTexture(*render.texture);
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
                                    world->addEntity(explosion);
                                    
                                    if (!(pos->x < -1.0f || pos->x > 1.0f || pos->y < -1.0f || pos->y > 1.0f)) {
                                        SoundManager::playSound("client/sound/explosion-91872.ogg");
                                    }
                                }
                            }
                            world->removeEntity(existingEntity);
                        }
                    }
                }
            } catch (const json::parse_error& e) {
                std::cerr << "Erreur parsing JSON 115: " << e.what() << std::endl;
            }
        }
    }

    while (networkManager.hasMessages()) {
        std::string message = networkManager.getNextMessage();
        std::string protocol_id = message.substr(1, 3);
        std::string content = message.substr(5);
        if (protocol_id == "114") {
            std::cout << "handleGameEvents" << std::endl;
            setState(MenuState::WIN);
        } else if (protocol_id == "133") {
            setState(MenuState::MAIN_MENU);
        } else if (protocol_id == "134") {
            setState(MenuState::MAIN_MENU);
        }
    }

    handleInGameKeyboardEvents();
    handleChangeSpritePlayer();
}
