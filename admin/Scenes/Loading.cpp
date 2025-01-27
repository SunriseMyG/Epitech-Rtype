#include "Loading.hpp"

LoadingScene::LoadingScene(SceneManager& sceneManager, FontLoader& fontLoader) 
    : Scene(sceneManager, fontLoader), 
        failedText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.i18n_.get("loading"), 48, sf::Color::White, sf::Vector2f(1920 / 2, 1080 / 2)) {
}

void LoadingScene::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
}

void LoadingScene::draw(sf::RenderWindow& window) {
    failedText.draw(window);
}
