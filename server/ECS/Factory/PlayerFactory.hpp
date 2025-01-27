#ifndef PLAYERFACTORY_HPP
    #define PLAYERFACTORY_HPP

    #include "../Entity/Entity.hpp"
    #include "../Component/Component.hpp"

    class PlayerFactory {
        public:
            static std::shared_ptr<Entity> createPlayer(int id, std::string type, int health, float x, float y, float speed, float hw, float hh, int ping = 0) {
                auto player = std::make_shared<Entity>(id, "PLAYER", type);

                player->addComponent(std::make_shared<HealthComponent>(health));
                player->addComponent(std::make_shared<PositionComponent>(x, y));
                player->addComponent(std::make_shared<VelocityComponent>(speed, speed));
                player->addComponent(std::make_shared<HitboxComponent>(hw, hh));
                player->addComponent(std::make_shared<ScoreComponent>(0));
                player->addComponent(std::make_shared<StateComponent>("CREATE"));
                player->addComponent(std::make_shared<PingComponent>(ping));
                player->addComponent(std::make_shared<PositionHistoryComponent>());

                return player;
            }
        private:
    };

#endif // PLAYERFACTORY_HPP