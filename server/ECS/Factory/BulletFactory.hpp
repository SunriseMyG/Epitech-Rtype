
#ifndef BULLETFACTORY_HPP
    #define BULLETFACTORY_HPP

    #include "../Entity/Entity.hpp"
    #include "../Component/Component.hpp"

    class BulletFactory {
        public:
            static std::shared_ptr<Entity> createBullet(int id, std::string group, std::string type, int damage, float x, float y, float speed, float hw, float hh, std::string pattern, std::string direction) {
                auto bullet = std::make_shared<Entity>(id, group, type);

                bullet->addComponent(std::make_shared<DamageComponent>(damage));
                bullet->addComponent(std::make_shared<PositionComponent>(x, y));
                bullet->addComponent(std::make_shared<VelocityComponent>(speed));
                bullet->addComponent(std::make_shared<HitboxComponent>(hw, hh));
                bullet->addComponent(std::make_shared<MovementComponent>(pattern, direction));
                bullet->addComponent(std::make_shared<StateComponent>("CREATE"));
                bullet->addComponent(std::make_shared<PositionHistoryComponent>());

                return bullet;
            }
        private:
    };

#endif // BULLETFACTORY_HPP