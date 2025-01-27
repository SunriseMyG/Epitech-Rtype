#ifndef IGAMEENGINE_HPP
    #define IGAMEENGINE_HPP

    #include <memory>
    #include <string>
    #include <functional>
    #include <utility>
    #include "../Room/Room.hpp"

    class IGameEngine {
        public:
            virtual ~IGameEngine() = default;

            virtual void run(std::shared_ptr<Room> room) = 0;
            virtual void update(std::shared_ptr<Room> room, const float &deltaTime) = 0;
            virtual void enemyGenerator(std::shared_ptr<Room> room, const float &deltaTime) = 0;
            virtual void playerGenerator(std::shared_ptr<Room> room) = 0;
            virtual std::pair<float, float> playerMoove(std::shared_ptr<Room> room, const int &playerId, const std::string &key) = 0;

            virtual void setFunctionsendEntitiesClients(std::function<void(std::shared_ptr<Room>)> sendEntitiesClientsFunction) = 0;
            virtual void setFunctionsendLevelClients(std::function<void(std::shared_ptr<Room>)> sendLevelClientsFunction) = 0;
            virtual void setFunctionsendEndGameClients(std::function<void(std::shared_ptr<Room>, bool win)> sendEndGameClientsFunction) = 0;
            virtual void setFunctionsendKickPlayer(std::function<void(int, std::string)> sendKickPlayerFunction) = 0;

            virtual void loadConfigForDifficulty(std::shared_ptr<Room> room) = 0;
    };

#endif // IGAMEENGINE_HPP
