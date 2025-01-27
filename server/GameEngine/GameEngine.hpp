#ifndef GAMEENGINE_HPP
    #define GAMEENGINE_HPP

    #include "IGameEngine.hpp"
    #include <vector>
    #include <map>
    #include <fstream>
    #include <sstream>
    #include <memory>
    #include <iostream>
    #include <nlohmann/json.hpp>
    #include "../Room/Room.hpp"
    #include "../ECS/Factory/EnemyFactory.hpp"
    #include "../ECS/Factory/PlayerFactory.hpp"
    #include "../ECS/Factory/BulletFactory.hpp"
    #include "../ECS/Factory/PowerupFactory.hpp"
    #include "../ECS/System/System.hpp"
    #include "../Database/Database.hpp"

    using json = nlohmann::json;

    struct SpawnInfo {
        std::string type;
        int spawnTimer;
        float x;
        float y;
    };

    struct LevelInfo {
        std::string name;
        std::vector<SpawnInfo> entities;
    };

    struct Hitbox {
        float width;
        float height;
    };

    struct Bullet {
        float speed;
        int damage;
        Hitbox hitbox;
        int spawnTimer;
    };

    struct Movement {
        std::string pattern;
        std::string direction;
    };

    struct EntityInfo {
        int health;
        float speed;
        float reward;
        Movement mouvement;
        Hitbox hitbox;
        std::string bulletType;
        int timeRemaining;
    };

    class GameEngine : public IGameEngine {
        public:
            GameEngine();
            ~GameEngine() override = default;

            void run(std::shared_ptr<Room> room) override;
            void update(std::shared_ptr<Room> room, const float &deltaTime) override;
            void enemyGenerator(std::shared_ptr<Room> room, const float &deltaTime) override;
            void playerGenerator(std::shared_ptr<Room> room) override;
            std::pair<float, float> playerMoove(std::shared_ptr<Room> room, const int &playerId, const std::string &key) override;

            void setFunctionsendEntitiesClients(std::function<void(std::shared_ptr<Room>)> sendEntitiesClientsFunction) override;
            void setFunctionsendLevelClients(std::function<void(std::shared_ptr<Room>)> sendLevelClientsFunction) override;
            void setFunctionsendEndGameClients(std::function<void(std::shared_ptr<Room>, bool win)> sendEndGameClientsFunction) override;
            void setFunctionsendKickPlayer(std::function<void(int, std::string)> sendKickPlayerFunction) override;

            void loadConfigForDifficulty(std::shared_ptr<Room> room) override;

        private:
            int maxLevel;
            std::map<std::string, LevelInfo> levels;
            std::map<std::string, EntityInfo> entityTable;
            std::map<std::string, Bullet> bulletTable;

            EnemyFactory enemyFactory;
            PlayerFactory playerFactory;
            BulletFactory bulletFactory;

            std::function<void(std::shared_ptr<Room>)> sendEntitiesClientsFunction;
            std::function<void(std::shared_ptr<Room>)> sendLevelClientsFunction;
            std::function<void(std::shared_ptr<Room>, bool win)> sendEndGameClientsFunction;
            std::function<void(int playerId, std::string message)> sendKickPlayerFunction;

            LinearSystem linearSystem;
            SinusoidalSystem sinusoidalSystem;
            AutoShootEnemy autoShootEnemy;
            BulletCollision bulletCollision;
            LevelSystem levelSystem;
            PowerupSystemDisappear powerupSystemDisappear;
            PowerupSystem powerupSystem;

            Database database_;

            // Difficulty

            std::map<std::string, LevelInfo> levelsEasy;
            std::map<std::string, EntityInfo> entityTableEasy;
            std::map<std::string, Bullet> bulletTableEasy;
            int maxLevelEasy;

            std::map<std::string, LevelInfo> levelsMedium;
            std::map<std::string, EntityInfo> entityTableMedium;
            std::map<std::string, Bullet> bulletTableMedium;
            int maxLevelMedium;

            std::map<std::string, LevelInfo> levelsHard;
            std::map<std::string, EntityInfo> entityTableHard;
            std::map<std::string, Bullet> bulletTableHard;
            int maxLevelHard;

            std::map<std::string, LevelInfo> *currentLevels;
            std::map<std::string, EntityInfo> *currentEntityTable;
            std::map<std::string, Bullet> *currentBulletTable;
            int *currentMaxLevel;

            void loadConfig(const std::string &levelFile, const std::string &entityFile, const std::string &bulletFile, std::map<std::string, LevelInfo> &levels, std::map<std::string, EntityInfo> &entityTable, std::map<std::string, Bullet> &bulletTable, int &maxLevel);
    };

#endif // GAMEENGINE_HPP
