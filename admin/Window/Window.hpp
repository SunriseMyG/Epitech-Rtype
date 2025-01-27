
#ifndef WINDOW_HPP_
    #define WINDOW_HPP_

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include "./../Scenes/SceneManager.hpp"
    #include "./../Scenes/Login.hpp"
    #include "./../Scenes/Failed.hpp"
    #include "./../Scenes/Loading.hpp"
    #include "./../Scenes/Global.hpp"
    #include "./../Scenes/Room.hpp"
    
    class Window {
        public:
            Window() = default;
            ~Window() = default;

            void createWindow(const std::string host, const std::string port);
            void manageWindowEvents(SceneManager& sceneManager);
            void displayScene(SceneManager& sceneManager);

        private:
            sf::RenderWindow window;
            sf::Event event;
            sf::VideoMode video_mode;
    };


#endif /* !WINDOW_HPP_ */