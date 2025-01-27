#include "Failed.hpp"

FailedScene::FailedScene(SceneManager& sceneManager, FontLoader& fontLoader) 
    : Scene(sceneManager, fontLoader), 
        failedText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.i18n_.get("login_failed"), 48, sf::Color::Red, sf::Vector2f(1920 / 2, 1080 / 2)) {
}

void FailedScene::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            sceneManager.switchTo("login");
        }
    }
}

void FailedScene::draw(sf::RenderWindow& window) {
    failedText.draw(window);
}
