
#ifndef POWERUPFACTORY_HPP
    #define POWERUPFACTORY_HPP

    #include "../Entity/Entity.hpp"
    #include "../Component/Component.hpp"

    class PowerupFactory {
        public:
            static std::shared_ptr<Entity> createPowerup(int id, std::string group, std::string type, float x, float y, float hw, float hh, int lifetime, float speed) {
                std::shared_ptr<Entity> powerup = std::make_shared<Entity>(id, group, type);

                powerup->addComponent(std::make_shared<PositionComponent>(x, y));
                powerup->addComponent(std::make_shared<HitboxComponent>(hw, hh));
                powerup->addComponent(std::make_shared<VelocityComponent>(speed, speed));
                powerup->addComponent(std::make_shared<TimerComponent>(std::chrono::seconds(lifetime)));
                powerup->addComponent(std::make_shared<StateComponent>("CREATE"));

                return powerup;
            }
        private:
    };

#endif // POWERUPFACTORY_HPP