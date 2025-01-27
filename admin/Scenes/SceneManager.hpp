#ifndef SCENEMANAGER_HPP
    #define SCENEMANAGER_HPP

    #include <SFML/Graphics.hpp>
    #include <memory>
    #include <map>
    #include <string>
    #include "./../I18n/i18n.hpp"
    #include "./../Utils/FontLoader.hpp"
    #include "./../Utils/CreateButton.hpp"
    #include "./../Utils/CreateInputField.hpp"
    #include "./../Utils/CreateCard.hpp"
    #include "./../Utils/CreateStatCard.hpp"
    #include "./../Utils/Flagsi18n.hpp"
    #include "./../Utils/Chat.hpp"
    #include "./../Network/Network.hpp"

    class SceneManager;

    class Scene {
        public:
            Scene(SceneManager& sceneManager, FontLoader& fontLoader) : sceneManager(sceneManager), fontLoader(fontLoader) {}
            virtual ~Scene() = default;
            virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window) = 0;
            virtual void draw(sf::RenderWindow& window) = 0;
            virtual void update() {};

        protected:
            SceneManager& sceneManager;
            FontLoader& fontLoader;
    };

    class SceneManager {
        public:
            SceneManager(sf::RenderWindow& window, FontLoader& fontLoader, const std::string& host, const std::string& port);
            ~SceneManager() = default;

            void handleEvent(const sf::Event& event);
            void draw();
            void addScene(const std::string& name, std::shared_ptr<Scene> scene);
            void switchTo(const std::string& name);
            Scene &getScene(const std::string &name);
            Network &getNetwork() { return network; }
            sf::RenderWindow& getWindow() { return window; }

            i18n i18n_;

        private:
            std::map<std::string, std::shared_ptr<Scene>> scenes;
            std::shared_ptr<Scene> currentScene;
            sf::RenderWindow& window;
            FontLoader& fontLoader;
            Network network;

            Flagsi18n flagsi18n;
    };

#endif // SCENEMANAGER_HPP