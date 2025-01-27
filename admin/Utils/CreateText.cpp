#include "CreateText.hpp"

CreateText::CreateText(const sf::Font& font, const std::string& text, unsigned int characterSize, const sf::Color& color, const sf::Vector2f& position) {
    this->text.setFont(font);
    this->text.setString(text);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(color);
    this->text.setPosition(position);
}

void CreateText::setText(const std::string& text) {
    this->text.setString(text);
}

void CreateText::setPosition(const sf::Vector2f& position) {
    this->text.setPosition(position);
}

void CreateText::setColor(const sf::Color& color) {
    this->text.setFillColor(color);
}

void CreateText::setCharacterSize(unsigned int size) {
    this->text.setCharacterSize(size);
}

void CreateText::draw(sf::RenderWindow& window) const {
    window.draw(this->text);
}

sf::FloatRect CreateText::getGlobalBounds() const {
    return this->text.getGlobalBounds();
}