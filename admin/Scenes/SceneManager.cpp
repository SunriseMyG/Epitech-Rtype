#include "SceneManager.hpp"

SceneManager::SceneManager(sf::RenderWindow& window, FontLoader& fontLoader, const std::string& host, const std::string& port)
    : window(window), fontLoader(fontLoader), currentScene(nullptr), network(host, port), i18n_(),
        flagsi18n(i18n_, sf::Vector2f(10, 10), sf::Vector2f(100, 50)) {
    i18n_.loadFromFile(i18n::Language::EN, "./admin/I18n/locales/en.json");
    i18n_.loadFromFile(i18n::Language::FR, "./admin/I18n/locales/fr.json");
    i18n_.setLanguage(i18n::Language::EN);
}

void SceneManager::handleEvent(const sf::Event& event) {
    if (currentScene) {
        if (flagsi18n.handleEvent(event, window))
            currentScene->update();

        currentScene->handleEvent(event, window);
    }
}

void SceneManager::draw() {
    flagsi18n.draw(window);
    if (currentScene) {
        currentScene->draw(window);
    }
}

void SceneManager::addScene(const std::string& name, std::shared_ptr<Scene> scene) {
    scenes[name] = scene;
}

void SceneManager::switchTo(const std::string& name) {
    auto it = scenes.find(name);
    if (it != scenes.end()) {
        currentScene = it->second;
    }
}

Scene &SceneManager::getScene(const std::string &name) {
    return *scenes[name];
}