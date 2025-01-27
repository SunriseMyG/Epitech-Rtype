#include "CreateButton.hpp"

CreateButton::CreateButton(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &buttonColor, const std::string &textString, const sf::Font &font, const sf::Color &textColor)
    : position(position), size(size), textString(textString), font(font), isButtonPressed(false) {
    button.setOrigin(size.x / 2, size.y / 2);
    button.setPosition(position);
    button.setSize(size);
    button.setFillColor(buttonColor);

    buttonText = new CreateText(font, textString, 24, textColor, sf::Vector2f(position.x - size.x / 2 + 5, position.y - size.y / 2 + 5));
}

void CreateButton::handleEvent(const sf::RenderWindow& window, const sf::Event &event, std::function<void()> onClick) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos);
            if (button.getGlobalBounds().contains(worldPos)) {
                isButtonPressed = true;
            }
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos);
            if (isButtonPressed && button.getGlobalBounds().contains(worldPos)) {
                if (onClick) {
                    onClick();
                } else {
                    std::cerr << "No function provided for button click event\n";
                }
            }
            isButtonPressed = false;
        }
    }
}

void CreateButton::draw(sf::RenderWindow& window) {
    window.draw(button);
    buttonText->draw(window);
}