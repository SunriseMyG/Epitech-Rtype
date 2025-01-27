#ifndef LOADING_HPP
    #define LOADING_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "SceneManager.hpp"
    #include <SFML/System/Clock.hpp>
    #include <SFML/System/Sleep.hpp>

    class LoadingScene : public Scene {
        public:
            LoadingScene(SceneManager& sceneManager, FontLoader& fontLoader);
            ~LoadingScene() = default;

            void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
            void draw(sf::RenderWindow& window) override;

        private:
            CreateText failedText;
    };

#endif // LOADING_HPP