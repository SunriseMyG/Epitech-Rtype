#ifndef SYSTEM_HPP
    #define SYSTEM_HPP

    #include <iostream>
    #include "../Component/Component.hpp"
    #include "../Entity/Entity.hpp"
    #include <cmath>
    #include "../../Room/Room.hpp"
    #include "../../Database/Database.hpp"
    #include "../Factory/BulletFactory.hpp"
    

    class System {
        public:
            System() = default;
            virtual ~System() = default;
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) = 0;
    };

    class LinearSystem : public System {
        public:
            LinearSystem() {}
            virtual ~LinearSystem() {}
        
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                auto movement = entity.getComponent<MovementComponent>();
        
                if (!movement || movement->pattern != "LINEAR" || entity.getGroup() == "PLAYER") {
                    return;
                }

                auto position = entity.getComponent<PositionComponent>();
                auto velocity = entity.getComponent<VelocityComponent>();
                auto hitbox = entity.getComponent<HitboxComponent>();
                auto history = entity.getComponent<PositionHistoryComponent>();
        
                if (position && velocity) {
                    entity.getComponent<StateComponent>()->state = "UPDATE";
        
                    if (history) {
                        history->addPosition(position->x, position->y, room->getCurrentTime());
                    }

                    if (movement->direction == "UP") {
                        position->y += velocity->y * deltaTime;
                        if (position->y - hitbox->y > 100) {
                            movement->direction = "DOWN";
                        }
                    } else if (movement->direction == "DOWN") {
                        position->y -= velocity->y * deltaTime;
                        if (position->y + hitbox->y < 0) {
                            movement->direction = "UP";
                        }
                    } else if (movement->direction == "LEFT") {
                        position->x -= velocity->x * deltaTime;
                        if (position->x + hitbox->x < 0) {
                            entity.markForDeletion();
                        }
                    } else if (movement->direction == "RIGHT") {
                        position->x += velocity->x * deltaTime;
                        if (position->x - hitbox->x > 100) {
                            entity.markForDeletion();
                        }
                    }
                }
            }
    };               

    class SinusoidalSystem : public System {
        public:
            SinusoidalSystem() {}
            virtual ~SinusoidalSystem() {}
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                auto movement = entity.getComponent<MovementComponent>();

                if (!movement || movement->pattern != "SINUSOIDAL" || entity.getGroup() == "PLAYER") {
                    return;
                }

                auto position = entity.getComponent<PositionComponent>();
                auto velocity = entity.getComponent<VelocityComponent>();
                auto history = entity.getComponent<PositionHistoryComponent>();

                if (position && velocity) {
                    entity.getComponent<StateComponent>()->state = "UPDATE";
        
                    if (history) {
                        history->addPosition(position->x, position->y, room->getCurrentTime());
                    }

                    if (movement->direction == "UP") {
                        position->y += velocity->y * deltaTime;
                        position->x += std::sin(position->y) * 10 * deltaTime;
                        if (position->y > 100 - velocity->y) {
                            position->y -= velocity->y * deltaTime;
                        } else if (position->y < 0 + velocity->y) {
                            position->y += velocity->y * deltaTime;
                        }
                    } else if (movement->direction == "DOWN") {
                        position->y -= velocity->y * deltaTime;
                        position->x += std::sin(position->y) * 10 * deltaTime;
                        if (position->y > 100 - velocity->y) {
                            position->y -= velocity->y * deltaTime;
                        } else if (position->y < 0 + velocity->y) {
                            position->y += velocity->y * deltaTime;
                        }
                    } else if (movement->direction == "LEFT") {
                        position->x -= velocity->x * deltaTime;
                        position->y += std::sin(position->x) * 10 * deltaTime;
                        if (position->x < 0) {
                            entity.markForDeletion();
                        }
                    } else if (movement->direction == "RIGHT") {
                        position->x += velocity->x * deltaTime;
                        position->y += std::sin(position->x) * 10 * deltaTime;
                        if (position->x > 100) {
                            entity.markForDeletion();
                        }
                    }
                }
            }
    };


    class CollisionBetweenEntities : public System {
        public:
            CollisionBetweenEntities() {}
            virtual ~CollisionBetweenEntities() {}
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                auto position = entity.getComponent<PositionComponent>();
                auto collision = entity.getComponent<HitboxComponent>();

                if (!position || !collision)
                    return;

                for (auto& other : room->getEntities()) {
                    if (other->getGroup() == entity.getGroup()) {
                        continue;
                    }

                    auto otherPosition = other->getComponent<PositionComponent>();
                    auto otherCollision = other->getComponent<HitboxComponent>();

                    if (!otherPosition || !otherCollision)
                        continue;

                    if (position->x < otherPosition->x + otherCollision->x &&
                        position->x + collision->x > otherPosition->x &&
                        position->y < otherPosition->y + otherCollision->y &&
                        position->y + collision->y > otherPosition->y) {
                        
                        if (entity.getGroup() == "PLAYER" && other->getGroup() == "ENEMY") {
                            position->updatePrevPos();
                            position->x = position->prev_x;
                            position->y = position->prev_y;
                        } else if (entity.getGroup() == "ENEMY" && other->getGroup() == "PLAYER") {
                            position->updatePrevPos();
                            position->x = position->prev_x;
                            position->y = position->prev_y;
                        }
                    }
                }
            }
    };

    class AutoShootEnemy : public System {
        public:
            AutoShootEnemy() {}
            virtual ~AutoShootEnemy() {}
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                if (entity.getGroup() != "ENEMY")
                    return;
    
                auto timerComponent = entity.getComponent<TimerComponent>();
                if (timerComponent) {
                    auto currentTime = std::chrono::steady_clock::now();
                    auto elapsedTime = std::chrono::duration<float>(currentTime - timerComponent->start_time);
                    auto targetTime = std::chrono::duration<float>(timerComponent->time);

                    if (elapsedTime.count() >= targetTime.count()) {
                        timerComponent->start_time = currentTime;
                        auto bullet = BulletFactory::createBullet(room->generateEntityID(), "ENEMY-BULLET", entity.getComponent<BulletDataComponent>()->type,
                            entity.getComponent<BulletDataComponent>()->damage, entity.getComponent<PositionComponent>()->x, entity.getComponent<PositionComponent>()->y,
                            entity.getComponent<BulletDataComponent>()->velocity.x, entity.getComponent<BulletDataComponent>()->hitbox.x,
                            entity.getComponent<BulletDataComponent>()->hitbox.y, entity.getComponent<BulletDataComponent>()->movement.pattern,
                            entity.getComponent<BulletDataComponent>()->movement.direction);
                        room->addEntity(bullet);
                        std::cout << "Enemy " << entity.getId() << " shot a bullet " << bullet->getId() << " (Damage: " << entity.getComponent<BulletDataComponent>()->damage << ")" << std::endl;
                    }
                }
            }
    };

    class PowerupSystemDisappear : public System {
        public:
            PowerupSystemDisappear() {}
            virtual ~PowerupSystemDisappear() {}
            virtual void update(std::shared_ptr<Room> room, Entity &entity, float deltaTime) {
                if (entity.getGroup() != "POWERUP")
                    return;

                auto timerComponent = entity.getComponent<TimerComponent>();
                if (timerComponent) {
                    auto currentTime = std::chrono::steady_clock::now();
                    auto elapsedTime = std::chrono::duration<float>(currentTime - timerComponent->start_time);
                    auto targetTime = std::chrono::duration<float>(timerComponent->time);

                    if (elapsedTime.count() >= targetTime.count()) {
                        // std::cout << "Powerup " << entity.getId() << " disappeared after " << elapsedTime.count() << " seconds" << std::endl;
                        entity.markForDeletion();
                    }
                }
            }
    };

    class PowerupSystem : public System {
        public:
            PowerupSystem() {}
            virtual ~PowerupSystem() {}
            virtual void update(std::shared_ptr<Room> room, Entity &entity, float deltaTime) override {
                for (auto &powerup : room->getEntities()) {
                    if (powerup->getType() == "POWERUP" && checkCollision(entity, *powerup)) {
                        auto timerComponent = powerup->getComponent<TimerComponent>();
                        applyPowerupEffect(entity, timerComponent);
                        room->removeEntity(powerup);
                        std::cout << "Player collected POWERUP at " << powerup->getComponent<PositionComponent>()->x << ", " << powerup->getComponent<PositionComponent>()->y << std::endl;
                        break;
                    }
                }
            }

        private:
            bool checkCollision(Entity &player, Entity &powerup) {

                if (player.getGroup() != "PLAYER" || powerup.getGroup() != "POWERUP" || player.getType() != "ENEMY-BULLET" || powerup.getType() != "PLAYER-BULLET")
                    return false;

                auto playerPos = player.getComponent<PositionComponent>();
                auto playerHitbox = player.getComponent<HitboxComponent>();
                auto powerupPos = powerup.getComponent<PositionComponent>();
                auto powerupHitbox = powerup.getComponent<HitboxComponent>();

                return (playerPos->x - playerHitbox->x / 2 < powerupPos->x + powerupHitbox->x / 2 &&
                        playerPos->x + playerHitbox->x / 2 > powerupPos->x - powerupHitbox->x / 2 &&
                        playerPos->y - playerHitbox->y / 2 < powerupPos->y + powerupHitbox->y / 2 &&
                        playerPos->y + playerHitbox->y / 2 > powerupPos->y - powerupHitbox->y / 2);
            }

            void applyPowerupEffect(Entity &player, std::shared_ptr<TimerComponent> timer) {
                if (player.getGroup() == "PLAYER") {
                    std::cout << "Player " << player.getId() << " collected a powerup" << std::endl;
                    player.getComponent<VelocityComponent>()->x = 2.5;
                    player.getComponent<VelocityComponent>()->y = 2.5;
                    if (timer->time.count() > 0)  {
                        auto currentTime = std::chrono::steady_clock::now();
                        auto elapsedTime = std::chrono::duration<float>(currentTime - timer->start_time);
                        auto targetTime = std::chrono::duration<float>(timer->time);

                        if (elapsedTime.count() >= targetTime.count()) {
                            player.getComponent<VelocityComponent>()->x = 1;
                            player.getComponent<VelocityComponent>()->y = 1;

                            std::cout << "Player " << player.getId() << " powerup effect ended" << std::endl;
                        }
                    }
                }
                return;
            }
    };

    class BulletCollision : public System {
        public:
            BulletCollision() {}
            virtual ~BulletCollision() {}
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                if (entity.getGroup() == "PLAYER-BULLET") {
                    auto position = entity.getComponent<PositionComponent>();
                    auto collision = entity.getComponent<HitboxComponent>();

                    if (!position || !collision)
                        return;
                        
                    for (auto& other : room->getEntities()) {
                        if (other->getGroup() != "ENEMY")
                            continue;

                        auto otherPosition = other->getComponent<PositionComponent>();
                        auto otherCollision = other->getComponent<HitboxComponent>();
                        auto otherHealth = other->getComponent<HealthComponent>();
                        
                        if (!otherPosition || !otherCollision || !otherHealth)
                            continue;

                        float entityLeft = position->x - collision->x / 2;
                        float entityRight = position->x + collision->x / 2;
                        float entityTop = position->y - collision->y / 2;
                        float entityBottom = position->y + collision->y / 2;

                        float otherLeft = otherPosition->x - otherCollision->x / 2;
                        float otherRight = otherPosition->x + otherCollision->x / 2;
                        float otherTop = otherPosition->y - otherCollision->y / 2;
                        float otherBottom = otherPosition->y + otherCollision->y / 2;

                        if (entityRight > otherLeft && entityLeft < otherRight && entityBottom > otherTop && entityTop < otherBottom) {
                            std::cout << "Player bullet " << entity.getId() << " collided with enemy " << other->getId() << "at position (" << position->x << ", " << position->y << ")" << std::endl;
                            entity.markForDeletion();
                            otherHealth->health -= entity.getComponent<DamageComponent>()->damage;
                            if (otherHealth->health <= 0) {
                                other->markForDeletion();
                                // STATISTIC
                                for (const auto &player : room->getPlayers()) {
                                    int nbrKills = database_.getNbrKillFromId("server/Database/table/statistic.json", player->getId());
                                    database_.setNbrKillFromId("server/Database/table/statistic.json", player->getId(), nbrKills + 1);
                                }
                                auto ownerComponent = entity.getComponent<OwnerComponent>();
                                if (ownerComponent) {
                                    for (auto& player : room->getEntities()) {
                                        if (player->getId() == ownerComponent->owner_id) {
                                            player->getComponent<ScoreComponent>()->score += other->getComponent<ScoreComponent>()->score;
                                            player->getComponent<StateComponent>()->state = "UPDATE";
                                        }
                                    }
                                }
                            }
                        }
                    }
                } else if (entity.getGroup() == "ENEMY-BULLET") {
                    auto position = entity.getComponent<PositionComponent>();
                    auto collision = entity.getComponent<HitboxComponent>();

                    if (!position || !collision)
                        return;

                    for (auto& other : room->getEntities()) {
                        if (other->getGroup() != "PLAYER")
                            continue;

                        auto otherPosition = other->getComponent<PositionComponent>();
                        auto otherCollision = other->getComponent<HitboxComponent>();
                        auto otherHealth = other->getComponent<HealthComponent>();

                        if (!otherPosition || !otherCollision || !otherHealth)
                            continue;

                        float entityLeft = position->x - collision->x / 2;
                        float entityRight = position->x + collision->x / 2;
                        float entityTop = position->y - collision->y / 2;
                        float entityBottom = position->y + collision->y / 2;

                        float otherLeft = otherPosition->x - otherCollision->x / 2;
                        float otherRight = otherPosition->x + otherCollision->x / 2;
                        float otherTop = otherPosition->y - otherCollision->y / 2;
                        float otherBottom = otherPosition->y + otherCollision->y / 2;

                        if (entityRight > otherLeft && entityLeft < otherRight && entityBottom > otherTop && entityTop < otherBottom && otherHealth->health > 0) {
                            entity.markForDeletion();
                            otherHealth->health -= entity.getComponent<DamageComponent>()->damage;
                            if (otherHealth->health <= 0) {
                                other->getComponent<HealthComponent>()->health = 0;
                                // STATISTIC
                                for (const auto &player : room->getPlayers()) {
                                    int nbrDeaths = database_.getNbrDeathFromId("server/Database/table/statistic.json", player->getId());
                                    database_.setNbrDeathFromId("server/Database/table/statistic.json", player->getId(), nbrDeaths + 1);
                                }
                                other->getComponent<StateComponent>()->state = "DELETE";
                            }
                        }
                    }
                }
            }

        private:
            Database database_;
    };

    class LevelSystem : public System {
        public:
            LevelSystem() {}
            virtual ~LevelSystem() {}
    
            void setSendLevelClientsFunction(std::function<void(std::shared_ptr<Room>)> func) {
                sendLevelClientsFunction = func;
            }
    
            virtual void update(std::shared_ptr<Room> room, Entity& entity, float deltaTime) {
                int enemyInLevel = room->getEnemyInLevel();
                int enemyDead = room->getEnemyDead();
    
                if (enemyInLevel != 0 && enemyDead == enemyInLevel) {
                    room->setEnemyDead(0);
                    room->setEnemyInLevel(0);
                    room->setLevel(room->getLevel() + 1);
                    room->resetLevelStartTime();
                    
                    for (auto& player : room->getPlayers()) {
                        auto entity = room->getEntityFromPlayer(player->getId());

                        if (entity.getComponent<HealthComponent>()->health <= 0)
                            entity.getComponent<StateComponent>()->state = "CREATE";

                        entity.getComponent<HealthComponent>()->health = 100;
                    }
    
                    std::cout << "Level " << room->getLevel() << " started" << std::endl;
    
                    if (sendLevelClientsFunction) {
                        sendLevelClientsFunction(room);
                    }
                }
            }
    
        private:
            std::function<void(std::shared_ptr<Room>)> sendLevelClientsFunction;
    };

#endif // SYSTEM_HPP