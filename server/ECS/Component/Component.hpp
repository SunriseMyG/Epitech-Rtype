
#ifndef COMPONENT_HPP
    #define COMPONENT_HPP

    #include <iostream>
    #include <string>
    #include <chrono>
    #include <deque>
    
    class Component {
        public:
            Component() {}
            virtual ~Component() {}
    };


    class PositionComponent : public Component {
        public:
            PositionComponent(float x = 0, float y = 0) : x(x), y(y), prev_x(x), prev_y(y) {}
            ~PositionComponent() {}

            float x;
            float y;

            float prev_x;
            float prev_y;

            void updatePrevPos() {
                prev_x = x;
                prev_y = y;
            }
    };

    class VelocityComponent : public Component {
        public:
            VelocityComponent(float x = 0, float y = 0) : x(x), y(y) {}
            ~VelocityComponent() {}

            float x;
            float y;
    };

    class HealthComponent : public Component {
        public:
            HealthComponent(int health = 100) : health(health) {}
            ~HealthComponent() {}

            int health;
    };

    class HitboxComponent : public Component {
        public:
            HitboxComponent(float x = 0, float y = 0) : x(x), y(y) {}
            ~HitboxComponent() {}

            float x;
            float y;
    };

    class ScoreComponent : public Component {
        public:
            ScoreComponent(int score = 0) : score(score) {}
            ~ScoreComponent() {}

            int score;
    };

    class DamageComponent : public Component {
        public:
            DamageComponent(int damage = 0) : damage(damage) {}
            ~DamageComponent() {}

            int damage;
    };

    class MovementComponent : public Component {
        public:
            MovementComponent(std::string pattern, std::string direction) : pattern(pattern), direction(direction) {}
            ~MovementComponent() {}

            std::string pattern;
            std::string direction;
    };

    class TimerComponent : public Component {
        public:
            TimerComponent(std::chrono::duration<float> time = std::chrono::duration<float>(0))
                : time(time), start_time(std::chrono::steady_clock::now()), target_time(start_time + std::chrono::duration_cast<std::chrono::steady_clock::duration>(time)) {}
            ~TimerComponent() {}
    
            std::chrono::duration<float> time;
            std::chrono::time_point<std::chrono::steady_clock> start_time;
            std::chrono::time_point<std::chrono::steady_clock> target_time;
    };

    class BulletDataComponent : public Component {
        public:
            BulletDataComponent(std::string type, int damage, float vx, float vy, float hw, float hh, std::string pattern, std::string direction)
                : type(type), damage(damage), velocity(vx, vy), hitbox(hw, hh), movement(pattern, direction) {}
            ~BulletDataComponent() {}

            std::string type;
            int damage;
            HitboxComponent hitbox;
            VelocityComponent velocity;
            MovementComponent movement;
    };

    class OwnerComponent : public Component {
        public:
            OwnerComponent(int owner_id) : owner_id(owner_id) {}
            ~OwnerComponent() {}

            int owner_id;
    };

    class StateComponent : public Component {
        public:
            StateComponent(std::string state) : state(state) {}
            ~StateComponent() {}

            std::string state;
    };

    class PingComponent : public Component {
        public:
            PingComponent(int ping) : currentPing(ping), lastUpdateTime(std::chrono::steady_clock::now()), advantage(true) {}
            ~PingComponent() {}
    
            void updatePing(int newPing) {
                auto currentTime = std::chrono::steady_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - lastUpdateTime).count();

                if (!advantage) {
                    newPing = 0;
                }
    
                if (elapsedTime >= 1) {
                    pingHistory.push_front(newPing);
                    if (pingHistory.size() > 30) {
                        pingHistory.pop_back(); 
                    }
                    currentPing = newPing;
                    lastUpdateTime = currentTime;
                }
            }

            void checkPing() {
                if (!advantage) {
                    return;
                }

                if (pingHistory.size() < 30) {
                    return;
                }
            
                int spikeCount = 0;
                int previousPing = pingHistory.front();
                
                for (size_t i = 1; i < pingHistory.size(); ++i) {
                    int currentPing = *std::next(pingHistory.begin(), i);
                    int difference = std::abs(currentPing - previousPing);
                    
                    if (difference > 100) {
                        spikeCount++;
                        if (spikeCount >= 5) {
                            std::cout << "5 ping spikes detected, advantage set to false" << std::endl;
                            advantage = false;
                        }
                    }
                    
                    previousPing = currentPing;
                }
                
                return;
            }
    
            int getCurrentPing() const { return currentPing; }
            const std::deque<int>& getPingHistory() const { return pingHistory; }
            std::chrono::steady_clock::time_point getLastUpdateTime() const { return lastUpdateTime; }
    
        private:
            int currentPing;
            std::deque<int> pingHistory;
            std::chrono::steady_clock::time_point lastUpdateTime;
            bool advantage;
    };

    struct PositionRecord {
        float x, y;
        float timestamp;
    };
    
    class PositionHistoryComponent : public Component {
        public:
            std::deque<PositionRecord> history;
            static const size_t MAX_HISTORY = 60;
        
            void addPosition(float x, float y, float timestamp) {
                history.push_back({x, y, timestamp});
                if (history.size() > MAX_HISTORY) {
                    history.pop_front();
                }
            }
        
            PositionRecord getPositionAt(float timestamp) {
                for (size_t i = 1; i < history.size(); ++i) {
                    if (history[i].timestamp >= timestamp) {
                        auto& prev = history[i - 1];
                        auto& curr = history[i];
                        float t = (timestamp - prev.timestamp) / (curr.timestamp - prev.timestamp);
                        return {prev.x + t * (curr.x - prev.x), prev.y + t * (curr.y - prev.y), timestamp};
                    }
                }
                return history.empty() ? PositionRecord{0, 0, timestamp} : history.back();
            }
    };

#endif // COMPONENT_HPP