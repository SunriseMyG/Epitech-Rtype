#include "Login.hpp"

LoginScene::LoginScene(SceneManager& sceneManager, FontLoader& fontLoader) 
    : Scene(sceneManager, fontLoader), 
        usernameField(sf::Vector2f(sceneManager.getWindow().getSize().x / 2, sceneManager.getWindow().getSize().y / 2), sf::Vector2f(400, 50), fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.i18n_.get("username")), 
        passwordField(sf::Vector2f(sceneManager.getWindow().getSize().x / 2, sceneManager.getWindow().getSize().y / 2 + 60), sf::Vector2f(400, 50), fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.i18n_.get("password")),
        loginButton(sf::Vector2f(1920 / 2, 1080 / 2 + 150), sf::Vector2f(400, 50), sf::Color::Green, sceneManager.i18n_.get("login"), fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"))
      {
}

void LoginScene::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    usernameField.handleEvent(event, window);
    passwordField.handleEvent(event, window);
    loginButton.handleEvent(window, event, [&]() {
        std::string username = usernameField.getText();
        std::string password = passwordField.getText();
        std::cout << "Login button clicked\n";
        std::cout << "Username: " << username << "\n";
        std::cout << "Password: " << password << "\n";
        

        if (sceneManager.getNetwork().isConnected()) {
            std::cout << "Sending login request to server\n";
            sceneManager.getNetwork().sendLogin(username, password);
            std::cout << "Login request sent\n";
            sceneManager.switchTo("loading");
            sceneManager.getNetwork().setLoginCallback([&](bool success) {
                if (success) {
                    sceneManager.getNetwork().sendGetRooms();
                    sceneManager.switchTo("global");
                } else {
                    sceneManager.switchTo("failed");
                }
            });
        } else {
            std::cerr << "Not connected to server" << std::endl;
        }
    });
}

void LoginScene::draw(sf::RenderWindow& window) {
    usernameField.draw(window);
    passwordField.draw(window);
    loginButton.draw(window);
}

void LoginScene::update() {
    usernameField.setPlaceholder(sceneManager.i18n_.get("username"));
    passwordField.setPlaceholder(sceneManager.i18n_.get("password")); 
    loginButton.setText(sceneManager.i18n_.get("login"));
}