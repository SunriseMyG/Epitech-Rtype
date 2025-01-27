#ifndef FAILED_HPP
    #define FAILED_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "SceneManager.hpp"
    #include <SFML/System/Clock.hpp>
    #include <SFML/System/Sleep.hpp>

    class FailedScene : public Scene {
        public:
            FailedScene(SceneManager& sceneManager, FontLoader& fontLoader);
            ~FailedScene() = default;

            void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
            void draw(sf::RenderWindow& window) override;

        private:
            CreateText failedText;
    };

#endif // FAILED_HPP