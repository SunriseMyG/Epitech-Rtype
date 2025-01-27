#ifndef IROOM_HPP
#define IROOM_HPP

#include <vector>
#include <memory>
#include <string>
#include "../Player/Player.hpp"
#include <chrono>
#include "../ECS/Entity/Entity.hpp"

class IRoom {
    public:
        enum State { WAITING, PLAYING, FINISHED };
        enum Difficulty { EASY, MEDIUM, HARD };
        virtual ~IRoom() = default;

        //Party
        virtual void startGame() = 0;
        virtual void endGame() = 0;

        //Setters
        virtual void setPassword(std::string password) = 0;
        virtual void setName(std::string name) = 0;
        virtual void setState(int state) = 0;
        virtual void setMaxPlayers(int maxPlayers) = 0;
        virtual void setScore(int score) = 0;
        virtual void setLevel(int level) = 0;
        virtual void setEnemyDead(int enemyDead) = 0;
        virtual void setEnemyInLevel(int enemyInLevel) = 0;
        virtual void resetLevelStartTime() = 0;
        virtual void setDifficulty(Difficulty difficulty) = 0;

        //Players
        virtual void addPlayer(std::shared_ptr<Player> player) = 0;
        virtual void removePlayer(std::shared_ptr<Player> player) = 0;

        //Entities
        virtual int generateEntityID() = 0;
        virtual std::vector<std::shared_ptr<Entity>> getEntities() = 0;
        virtual void addEntity(std::shared_ptr<Entity> entity) = 0;
        virtual void removeEntity(std::shared_ptr<Entity> entity) = 0;
        virtual void setEntityToPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) = 0;
        virtual void removeEntityFromPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) = 0;
        virtual Entity getEntityFromPlayer(int playerId) = 0;

        //Getters
        virtual std::vector<std::shared_ptr<Player>> getPlayers() = 0;
        virtual int getId() = 0;
        virtual std::string getName() = 0;
        virtual std::string getPassword() = 0;
        virtual State getState() = 0;
        virtual int getMaxPlayers() = 0;
        virtual int getScore() = 0;
        virtual int getLevel() = 0;
        virtual int getEnemyDead() = 0;
        virtual int getEnemyInLevel() = 0;
        virtual std::chrono::time_point<std::chrono::high_resolution_clock> getLevelStartTime() = 0;
        virtual bool isPrivate() = 0;
        virtual Difficulty getDifficulty() = 0;
        virtual int getPlayerIdFromEntity(std::shared_ptr<Entity> entity) = 0;
};

#endif // IROOM_HPP