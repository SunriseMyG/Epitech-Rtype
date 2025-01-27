#include "Window.hpp"

void Window::createWindow(const std::string host, const std::string port)
{
    FontLoader& fontLoader = FontLoader::getInstance();
    SceneManager sceneManager(window, fontLoader, host, port);

    try {
        sceneManager.getNetwork().connect();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        exit(84);
    }

    window.create(sf::VideoMode(800, 600), "Admin-Panel");
    
    sceneManager.addScene("login", std::make_shared<LoginScene>(sceneManager, fontLoader));
    sceneManager.addScene("failed", std::make_shared<FailedScene>(sceneManager, fontLoader));
    sceneManager.addScene("loading", std::make_shared<LoadingScene>(sceneManager, fontLoader));
    sceneManager.addScene("global", std::make_shared<GlobalScene>(sceneManager, fontLoader));
    sceneManager.addScene("room", std::make_shared<RoomScene>(sceneManager, fontLoader));
    sceneManager.switchTo("login");

    while (window.isOpen()) {
        manageWindowEvents(sceneManager);
        window.clear(sf::Color::Black);
        displayScene(sceneManager);
        window.display();
    }
}

void Window::manageWindowEvents(SceneManager& sceneManager)
{
    while (this->window.pollEvent(event)) {
        sceneManager.handleEvent(event);
        if (event.type == sf::Event::Closed) {
            this->window.close();
            exit(0);
        }
    }
}

void Window::displayScene(SceneManager& sceneManager)
{
    sceneManager.draw();
}
