#include "GameEngine.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

GameEngine::GameEngine()
{
    loadConfig("server/Config/Easy/levels_easy.json", "server/Config/Easy/entitys_easy.json", "server/Config/Easy/bullets_easy.json", levelsEasy, entityTableEasy, bulletTableEasy, maxLevelEasy);
    loadConfig("server/Config/Medium/levels_medium.json", "server/Config/Medium/entitys_medium.json", "server/Config/Medium/bullets_medium.json", levelsMedium, entityTableMedium, bulletTableMedium, maxLevelMedium);
    loadConfig("server/Config/Hard/levels_hard.json", "server/Config/Hard/entitys_hard.json", "server/Config/Hard/bullets_hard.json", levelsHard, entityTableHard, bulletTableHard, maxLevelHard);

    std::cout << "GameEngine initialized" << std::endl;
}

void GameEngine::loadConfig(const std::string& levelFile, const std::string& entityFile, const std::string& bulletFile, std::map<std::string, LevelInfo>& levels, std::map<std::string, EntityInfo>& entityTable, std::map<std::string, Bullet>& bulletTable, int& maxLevel) {
    std::ifstream file(levelFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << levelFile << std::endl;
        exit(84);
    }

    json j;
    file >> j;

    for (auto& [key, value] : j.items()) {
        LevelInfo level;
        level.name = value["name"];
        for (const auto& entityData : value["entities"]) {
            SpawnInfo entity;
            entity.type = entityData["type"];
            entity.spawnTimer = entityData["spawnTimer"];
            entity.x = entityData["x"];
            entity.y = entityData["y"];
            level.entities.push_back(entity);
        }
        levels[key] = level;
    }

    file.close();

    maxLevel = levels.size();
    std::cout << "Max level: " << maxLevel << std::endl;

    std::ifstream file2(entityFile);
    if (!file2.is_open()) {
        std::cerr << "Error: Could not open file " << entityFile << std::endl;
        exit(84);
    }

    json j2;
    file2 >> j2;

    for (auto& [key, value] : j2.items()) {
        EntityInfo entityInfo;
        entityInfo.health = value["health"];
        entityInfo.speed = value["speed"];
        entityInfo.reward = value["reward"];
        entityInfo.mouvement.pattern = value["mouvement"]["pattern"];
        entityInfo.mouvement.direction = value["mouvement"]["direction"];
        entityInfo.hitbox.width = value["hitbox"]["width"];
        entityInfo.hitbox.height = value["hitbox"]["height"];
        entityInfo.bulletType = value["bulletType"];
        entityInfo.timeRemaining = value["timeRemaining"];
        entityTable[key] = entityInfo;
    }

    file2.close();

    std::ifstream file3(bulletFile);
    if (!file3.is_open()) {
        std::cerr << "Error: Could not open file " << bulletFile << std::endl;
        exit(84);
    }

    json j3;
    file3 >> j3;

    for (auto& [key, value] : j3.items()) {
        Bullet bullet;
        bullet.speed = value["speed"];
        bullet.damage = value["damage"];
        bullet.hitbox.width = value["hitbox"]["width"];
        bullet.hitbox.height = value["hitbox"]["height"];
        bullet.spawnTimer = value["spawnTimer"];
        bulletTable[key] = bullet;
    }

    file3.close();

    // print level

    for (const auto& [key, value] : levels) {
        std::cout << "Level " << key << ": " << value.name << std::endl;
        for (const auto& entity : value.entities) {
            std::cout << "  - " << entity.type << " at " << entity.x << ", " << entity.y << " (" << entity.spawnTimer << "s)" << std::endl;
        }
    }
}

void GameEngine::loadConfigForDifficulty(std::shared_ptr<Room> room) {
    std::cout << "Loading config for difficulty " << room->getDifficulty() << std::endl;
    switch (room->getDifficulty()) {
        case Room::EASY:
            currentLevels = &levelsEasy;
            currentEntityTable = &entityTableEasy;
            currentBulletTable = &bulletTableEasy;
            currentMaxLevel = &maxLevelEasy;
            break;
        case Room::MEDIUM:
            currentLevels = &levelsMedium;
            currentEntityTable = &entityTableMedium;
            currentBulletTable = &bulletTableMedium;
            currentMaxLevel = &maxLevelMedium;
            break;
        case Room::HARD:
            currentLevels = &levelsHard;
            currentEntityTable = &entityTableHard;
            currentBulletTable = &bulletTableHard;
            currentMaxLevel = &maxLevelHard;
            break;
        default:
            currentLevels = &levelsEasy;
            currentEntityTable = &entityTableEasy;
            currentBulletTable = &bulletTableEasy;
            currentMaxLevel = &maxLevelEasy;
            break;
    }

    for (const auto& [key, value] : *currentLevels) {
        std::cout << "Level " << key << ": " << value.name << std::endl;
        for (const auto& entity : value.entities) {
            std::cout << "  " << entity.type << " at " << entity.x << ", " << entity.y << " (" << entity.spawnTimer << "s)" << std::endl;
        }
    }
}

void GameEngine::run(std::shared_ptr<Room> room)
{
    std::thread game_thread([this, room]() {
        try {
            // STATISTIC
            for (const auto &player : room->getPlayers()) {
                int nbrGames = database_.getNbrGameFromId("server/Database/table/statistic.json", player->getId());
                database_.setNbrGameFromId("server/Database/table/statistic.json", player->getId(), nbrGames + 1);
            }

            const float tickRate = 1.0f / 60.0f; // 60 ticks par seconde
            auto previousTime = std::chrono::high_resolution_clock::now();
        
            loadConfigForDifficulty(room);
            playerGenerator(room);
            room->resetLevelStartTime();
            while (room->getState() == IRoom::State::PLAYING) {
                auto currentTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<float> elapsedTime = currentTime - previousTime;
        
                if (elapsedTime.count() >= tickRate) {
                    previousTime = currentTime;
                    sendEntitiesClientsFunction(room);
                    update(room, tickRate);
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }

            for (auto player : room->getPlayers()) {
                auto entityPlayer = room->getEntityFromPlayer(player->getId());

                database_.setNewScoreInLeaderBoard("server/Database/table/leaderboards.json", database_.getUsernameFromId(player->getId()), entityPlayer.getComponent<ScoreComponent>()->score);
                room->removePlayer(player);
                std::cout << "Player " << player->getId() << " left room " << room->getId() << "." << std::endl;
                break;
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception in game thread: " << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception in game thread" << std::endl;
        }
    });
    game_thread.detach();
}

void GameEngine::update(std::shared_ptr<Room> room, const float &deltaTime)
{
    auto players = room->getPlayers();
    int playerCount = players.size();
    int playerAlive = 0;

    for (auto& player : players) {
        auto entity = room->getEntityFromPlayer(player->getId());
        entity.getComponent<PingComponent>()->updatePing(player->getPing());
        entity.getComponent<PingComponent>()->checkPing();
    }

    room->update(deltaTime);
    
    for (auto& entity : room->getEntities()) {
        if (entity->isMarkedForDeletion()) {
            if (entity->getGroup() == "ENEMY")
                room->setEnemyDead(room->getEnemyDead() + 1);
            
            room->removeEntity(entity);
        }

        linearSystem.update(room, *entity, deltaTime);
        sinusoidalSystem.update(room, *entity, deltaTime);
        bulletCollision.update(room, *entity, deltaTime);
        autoShootEnemy.update(room, *entity, deltaTime);
        levelSystem.update(room, *entity, deltaTime);
        powerupSystemDisappear.update(room, *entity, deltaTime);
        powerupSystem.update(room, *entity, deltaTime);

        if (entity->getGroup() == "PLAYER" && entity->getComponent<HealthComponent>()->health > 0)
            playerAlive++;
    }

    if (playerAlive == 0) {
        room->setState(IRoom::State::FINISHED);
        sendEndGameClientsFunction(room, false);
        std::cout << "Game over" << std::endl;
        // STATISTIC
        for (const auto &player : room->getPlayers()) {
            int nbrLosses = database_.getNbrLoseFromId("server/Database/table/statistic.json", player->getId());
            database_.setNbrLoseFromId("server/Database/table/statistic.json", player->getId(), nbrLosses + 1);
        }
    }
    
    enemyGenerator(room, deltaTime);
}

void GameEngine::enemyGenerator(std::shared_ptr<Room> room, const float &deltaTime)
{
    static auto lastSpawnTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsedTime = currentTime - room->getLevelStartTime();
    std::chrono::duration<float> timeSinceLastSpawn = currentTime - lastSpawnTime;

    if (timeSinceLastSpawn.count() >= 1.0f) {
        for (const auto& entity : (*currentLevels)[std::to_string(room->getLevel())].entities) {
            if (entity.type == "POWERUP") {
                if (static_cast<int>(elapsedTime.count()) == entity.spawnTimer) {
                    EntityInfo entityInfo = (*currentEntityTable)[entity.type];
                    auto powerup = PowerupFactory::createPowerup(room->generateEntityID(), "POWERUP", entity.type, entity.x, entity.y, entityInfo.hitbox.width, entityInfo.hitbox.height, entityInfo.timeRemaining, entityInfo.speed);
                    room->addEntity(powerup);
                    room->setEnemyDead(room->getEnemyDead() + 1);
                    std::cout << "Spawn POWERUP at " << entity.x << ", " << entity.y << "(" << entity.spawnTimer << "s)" << std::endl;
                }
                continue;
            }
            if (static_cast<int>(elapsedTime.count()) == entity.spawnTimer) {
                EntityInfo entityInfo = (*currentEntityTable)[entity.type];
                Bullet bulletInfo = (*currentBulletTable)[entityInfo.bulletType];
                auto enemy = enemyFactory.createEnemy(room->generateEntityID(), entity.type, entityInfo.health, entity.x, entity.y, entityInfo.speed,
                    entityInfo.hitbox.width, entityInfo.hitbox.height, entityInfo.mouvement.pattern, entityInfo.mouvement.direction, bulletInfo.spawnTimer,
                    entityInfo.bulletType, bulletInfo.damage, bulletInfo.speed, bulletInfo.speed, bulletInfo.hitbox.width,
                    bulletInfo.hitbox.height, entityInfo.reward);
                room->addEntity(enemy);
                std::cout << "Spawn " << entity.type << " at " << entity.x << ", " << entity.y << "(" << entity.spawnTimer << "s)" << std::endl;
            }
        }
        lastSpawnTime = currentTime;

        if (*currentMaxLevel + 1 == room->getLevel()) {
            room->setState(IRoom::State::FINISHED);
            sendEndGameClientsFunction(room, true);
            std::cout << "Win" << std::endl;
            // STATISTIC
            for (const auto &player : room->getPlayers()) {
                int nbrWins = database_.getNbrWinFromId("server/Database/table/statistic.json", player->getId());
                database_.setNbrWinFromId("server/Database/table/statistic.json", player->getId(), nbrWins + 1);
            }
            return;
        }

        room->setEnemyInLevel((*currentLevels)[std::to_string(room->getLevel())].entities.size());
    }
}

void GameEngine::playerGenerator(std::shared_ptr<Room> room)
{
    for (const auto& player : room->getPlayers()) {
        int entityID = room->generateEntityID();
        EntityInfo entityInfo = (*currentEntityTable)["PLAYER"];

        std::stringstream ss;
        ss << std::setw(5) << std::setfill('0') << entityID;
        std::string seed = ss.str();
        std::seed_seq seed2(seed.begin(), seed.end());
        std::mt19937 gen(seed2);

        std::uniform_int_distribution<> distY(0, 99);
        std::uniform_int_distribution<> distX(0, 9);

        int y = distY(gen);
        int x = distX(gen);

        auto newPlayer = playerFactory.createPlayer(entityID, "player", entityInfo.health, x, y, 
            entityInfo.speed, entityInfo.hitbox.width, entityInfo.hitbox.height, player->getPing());
        room->addEntity(newPlayer);
        room->setEntityToPlayer(player, newPlayer);
        std::cout << "Player " << player->getId() << " has entity " << entityID << std::endl;
    }
}

std::pair<float, float> GameEngine::playerMoove(std::shared_ptr<Room> room, const int &playerId, const std::string &key)
{
    auto player = room->getEntityFromPlayer(playerId);

    std::cout << "Player " << playerId << " pressed " << key << std::endl;

    if (player.getComponent<PositionComponent>()->y > 105 ||
        player.getComponent<PositionComponent>()->y < -5 ||
        player.getComponent<PositionComponent>()->x > 105 ||
        player.getComponent<PositionComponent>()->x < -5) {
        room->kickPlayer(playerId);
        sendKickPlayerFunction(playerId, "out of bounds");
        std::cout << "Player " << playerId << " has been kicked (out of bounds)" << std::endl;
        return std::make_pair(0, 0);
    }

    if (player.getComponent<VelocityComponent>()->x > 3 || player.getComponent<VelocityComponent>()->y > 3) {
        room->kickPlayer(playerId);
        sendKickPlayerFunction(playerId, "speed hack");
        std::cout << "Player " << playerId << " has been kicked (speed hack)" << std::endl;
        return std::make_pair(0, 0);
    }

    if (key == "UP" && (player.getComponent<PositionComponent>()->y - player.getComponent<HitboxComponent>()->y / 2 >= 0)) {
        player.getComponent<PositionComponent>()->y -= player.getComponent<VelocityComponent>()->y;
    } else if (key == "DOWN" && (player.getComponent<PositionComponent>()->y + player.getComponent<HitboxComponent>()->y / 2 <= 100)) {
        player.getComponent<PositionComponent>()->y += player.getComponent<VelocityComponent>()->y;
    } else if (key == "LEFT" && (player.getComponent<PositionComponent>()->x - player.getComponent<HitboxComponent>()->x / 2 >= 0)) {
        player.getComponent<PositionComponent>()->x -= player.getComponent<VelocityComponent>()->x;
    } else if (key == "RIGHT" && (player.getComponent<PositionComponent>()->x + player.getComponent<HitboxComponent>()->x / 2 <= 100)) {
        player.getComponent<PositionComponent>()->x += player.getComponent<VelocityComponent>()->x;
    } else if (key == "SPACE") {
        Bullet bulletInfo = (*currentBulletTable)[(*currentEntityTable)["PLAYER"].bulletType];

        auto ping = player.getComponent<PingComponent>()->getCurrentPing();
        auto position = player.getComponent<PositionHistoryComponent>()->getPositionAt(room->getCurrentTime() - ping);
        auto positionX = position.x;
        auto positionY = position.y;    
    
        auto bullet = bulletFactory.createBullet(room->generateEntityID(), "PLAYER-BULLET", entityTable["PLAYER"].bulletType, bulletInfo.damage,
            positionX, positionY, bulletInfo.speed, bulletInfo.hitbox.width,
            bulletInfo.hitbox.height, "LINEAR", "RIGHT");
    
        bullet->addComponent(std::make_shared<OwnerComponent>(player.getId()));
    
        room->addEntity(bullet);
        std::cout << "Player " << playerId << " shot a bullet" << std::endl;
    }
    
    player.getComponent<StateComponent>()->state = "UPDATE";
    player.getComponent<PositionHistoryComponent>()->addPosition(player.getComponent<PositionComponent>()->x, player.getComponent<PositionComponent>()->y, room->getCurrentTime());
    
    return std::make_pair(player.getComponent<PositionComponent>()->x, player.getComponent<PositionComponent>()->y);
}

void GameEngine::setFunctionsendEntitiesClients(std::function<void(std::shared_ptr<Room>)> function)
{
    this->sendEntitiesClientsFunction = function;
}

void GameEngine::setFunctionsendLevelClients(std::function<void(std::shared_ptr<Room>)> function)
{
    this->sendLevelClientsFunction = function;
    levelSystem.setSendLevelClientsFunction(function);
}

void GameEngine::setFunctionsendEndGameClients(std::function<void(std::shared_ptr<Room>, bool win)> function)
{
    this->sendEndGameClientsFunction = function;
}

void GameEngine::setFunctionsendKickPlayer(std::function<void(int, std::string)> function)
{
    this->sendKickPlayerFunction = function;
}