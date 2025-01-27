#ifndef LOGIN_HPP
    #define LOGIN_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "SceneManager.hpp"

    class LoginScene : public Scene {
        public:
            LoginScene(SceneManager& sceneManager, FontLoader& fontLoader);
            ~LoginScene() = default;

            void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
            void draw(sf::RenderWindow& window) override;
            void update() override;

        private:
            CreateInputField usernameField;
            CreateInputField passwordField;
            CreateButton loginButton;
    };

#endif // LOGIN_HPP