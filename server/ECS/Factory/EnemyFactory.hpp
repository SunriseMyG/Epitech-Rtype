
#ifndef ENEMYFACTORY_HPP
    #define ENEMYFACTORY_HPP

    #include "../Entity/Entity.hpp"
    #include "../Component/Component.hpp"

    class EnemyFactory {
        public:
            static std::shared_ptr<Entity> createEnemy(int id, std::string type, int health, float x, float y, float speed,
                float hw, float hh, std::string pattern = "LINEAR", std::string direction = "LEFT", int shootTimer = 0,
                std::string bulletType = "BASIC", int bulletDamage = 10, float bulletVx = 1, float bulletVy = 1, float bulletHW = 1, float bulletHH = 1, int reward = 0) {
                auto enemy = std::make_shared<Entity>(id, "ENEMY", type);

                enemy->addComponent(std::make_shared<HealthComponent>(health));
                enemy->addComponent(std::make_shared<PositionComponent>(x, y));
                enemy->addComponent(std::make_shared<VelocityComponent>(speed, speed));
                enemy->addComponent(std::make_shared<HitboxComponent>(hw, hh));
                enemy->addComponent(std::make_shared<MovementComponent>(pattern, direction));
                enemy->addComponent(std::make_shared<TimerComponent>(std::chrono::seconds(shootTimer)));
                enemy->addComponent(std::make_shared<BulletDataComponent>(bulletType, bulletDamage, bulletVx, bulletVy, bulletHW, bulletHH, "LINEAR", "LEFT"));
                enemy->addComponent(std::make_shared<ScoreComponent>(reward));
                enemy->addComponent(std::make_shared<StateComponent>("CREATE"));
                enemy->addComponent(std::make_shared<PositionHistoryComponent>());

                return enemy;
            }
        private:
    };

#endif // ENEMYFACTORY_HPP