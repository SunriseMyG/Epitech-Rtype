#ifndef ROOM_HPP
#define ROOM_HPP

#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include "IRoom.hpp"
#include "../Player/Player.hpp"

class Room : public IRoom {
    private:
        std::vector<std::shared_ptr<Player>> players;
        int id;
        std::string name;
        std::string password;
        State state;
        int maxPlayers;
        int score;
        int level;
        int entityID;
        int enemyDead;
        int enemyInLevel;
        std::chrono::time_point<std::chrono::high_resolution_clock> levelStartTime;
        std::vector<std::shared_ptr<Entity>> entities;
        std::vector<std::pair<int, std::shared_ptr<Entity>>> playerEntities;
        bool privateRoom;
        Difficulty difficulty_;
        float elapsedTime = 0.0f;
        std::vector<std::shared_ptr<Player>> players_;

    public:
        Room(int id, std::string name, std::string password, State state, int maxPlayers, bool privateRoom = false);
        ~Room();

        //Party
        void startGame() override;
        void endGame() override;

        //Setters
        void setPassword(std::string password) override;
        void setName(std::string name) override;
        void setState(int state) override;
        void setMaxPlayers(int maxPlayers) override;
        void setScore(int score) override;
        void setLevel(int level) override;
        void setEnemyDead(int enemyDead) override;
        void setEnemyInLevel(int enemyInLevel) override;
        void resetLevelStartTime() override;
        void setDifficulty(Difficulty difficulty) override;

        //Players
        void addPlayer(std::shared_ptr<Player> player) override;
        void removePlayer(std::shared_ptr<Player> player) override;

        //Entities
        int generateEntityID() override;
        std::vector<std::shared_ptr<Entity>> getEntities() override;
        void addEntity(std::shared_ptr<Entity> entity) override;
        void removeEntity(std::shared_ptr<Entity> entity) override;
        void setEntityToPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) override;
        void removeEntityFromPlayer(std::shared_ptr<Player> player, std::shared_ptr<Entity> entity) override;
        Entity getEntityFromPlayer(int playerId) override;

        //Getters
        std::vector<std::shared_ptr<Player>> getPlayers() override;
        int getId() override;
        std::string getName() override;
        std::string getPassword() override;
        IRoom::State getState() override;
        int getMaxPlayers() override;
        int getScore() override;
        int getLevel() override;
        int getEnemyDead() override;
        int getEnemyInLevel() override;
        std::chrono::time_point<std::chrono::high_resolution_clock> getLevelStartTime() override;
        bool isPrivate() override;
        Difficulty getDifficulty() override;
        int getPlayerIdFromEntity(std::shared_ptr<Entity> entity) override;
        void update(float deltaTime);
        float getCurrentTime() const;

        //Moderator
        void kickPlayer(std::shared_ptr<Player> player);
        void kickPlayer(int playerId);
};

#endif // ROOM_HPP