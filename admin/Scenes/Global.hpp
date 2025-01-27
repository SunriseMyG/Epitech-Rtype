#ifndef GLOBAL_HPP
    #define GLOBAL_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "SceneManager.hpp"
    #include "Room.hpp"
    #include <SFML/System/Clock.hpp>
    #include <SFML/System/Sleep.hpp>

    class GlobalScene : public Scene {
        public:
            GlobalScene(SceneManager& sceneManager, FontLoader& fontLoader);
            ~GlobalScene() = default;

            void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
            void draw(sf::RenderWindow& window) override;
            void loader();
            void update() override;
            std::string getStates(int number);

        private:
            bool loaded = false;
            std::vector<CreateCard> roomCards;
            CreateButton refreshButton;
            sf::Clock refreshCooldown;
            const float REFRESH_COOLDOWN_SECONDS = 2.0f;
            Chat chat_;
    };

#endif // GLOBAL_HPP