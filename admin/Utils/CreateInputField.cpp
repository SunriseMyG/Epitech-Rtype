#include "CreateInputField.hpp"

CreateInputField::CreateInputField(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, const std::string& placeholder)
    : placeholder(placeholder), isActive(false), showCursor(false),
      text(font, placeholder, 24, sf::Color(150, 150, 150), sf::Vector2f(position.x - size.x / 2 + 5, position.y - size.y / 2 + 5)) {
    rectangle.setOrigin(size.x / 2, size.y / 2);
    rectangle.setPosition(position);
    rectangle.setSize(size);
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);

    cursor.setSize(sf::Vector2f(2, size.y - 10));
    cursor.setFillColor(sf::Color::Black);
    cursor.setPosition(position.x + 5, position.y + 5);
}

void CreateInputField::handleEvent(const sf::Event &event, const sf::RenderWindow &window)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos);
        if (rectangle.getGlobalBounds().contains(worldPos)) {
            isActive = true;
            rectangle.setFillColor(sf::Color(200, 200, 255));
        } else {
            isActive = false;
            rectangle.setFillColor(sf::Color::White);
        }
    }

    if (isActive && event.type == sf::Event::TextEntered) {
        sf::Time currentTime = clock.getElapsedTime();
        if (currentTime - lastInputTime > sf::milliseconds(100)) {
            if (event.text.unicode == 8 && !userInput.empty()) {
                userInput.pop_back();
            } else if (event.text.unicode >= 32 && event.text.unicode <= 126) {
                userInput += static_cast<char>(event.text.unicode);
            }
            text.setText(userInput.empty() ? placeholder : userInput);
            text.setColor(userInput.empty() ? sf::Color(150, 150, 150) : sf::Color::Black);
            lastInputTime = currentTime;
        }
    }
}

void CreateInputField::draw(sf::RenderWindow& window) {
    window.draw(rectangle);
    text.draw(window);

    if (isActive) {
        sf::Time currentTime = cursorClock.getElapsedTime();
        if (currentTime.asMilliseconds() % 1000 < 500) {
            showCursor = true;
        } else {
            showCursor = false;
        }

        if (showCursor) {
            cursor.setPosition(text.getGlobalBounds().left + text.getGlobalBounds().width + 5, text.getPosition().y);
            window.draw(cursor);
        }
    }
}

std::string CreateInputField::getText() {
    return userInput;
}