#include "CreateCard.hpp"

CreateCard::CreateCard(const sf::Vector2f &position,
    const sf::Vector2f &size,
    const sf::Color &cardColor,
    const sf::Font &font,
    const std::string &leftCornerText,
    const std::string &rightCornerText,
    const std::string &rightCornerButtonText,
    const std::string id,
    const std::string roomName,
    const std::string roomPassword,
    const std::string roomState,
    const int maxPlayers,
    const int currentPlayers)
: position(position)
, size(size)
, leftCornerTextString(leftCornerText)
, rightCornerTextString(rightCornerText)
, rightCornerButtonTextString(rightCornerButtonText)
, font(font)
, id(id)
, roomName(roomName)
, roomPassword(roomPassword)
, roomState(roomState)
, maxPlayers(maxPlayers)
, currentPlayers(currentPlayers)
{
    card.setOrigin(size.x / 2, size.y / 2);
    card.setPosition(position);
    card.setSize(size);
    card.setFillColor(sf::Color::Transparent);
    card.setOutlineThickness(2.0f);
    card.setOutlineColor(cardColor);

    this->leftCornerText = new CreateText(font, leftCornerText, 24, sf::Color::White, sf::Vector2f(position.x - size.x / 2 + 5, position.y - size.y / 2 + 5));
    this->rightCornerText = new CreateText(font, rightCornerText, 24, sf::Color::White, 
        sf::Vector2f(position.x + (size.x / 2) - 50, position.y - (size.y / 2)));
    rightCornerButton = new CreateButton(sf::Vector2f(position.x + (size.x / 2) - 75, position.y + (size.y / 2) - 25), sf::Vector2f(150, 50), sf::Color(128, 128, 128), rightCornerButtonText, font, sf::Color::White);
}

void CreateCard::handleEvent(const sf::RenderWindow& window, const sf::Event &event, std::function<void()> onClick) {
    rightCornerButton->handleEvent(window, event, onClick);
}

void CreateCard::draw(sf::RenderWindow& window) {
    window.draw(card);
    leftCornerText->draw(window);
    rightCornerText->draw(window);
    rightCornerButton->draw(window);
}

void CreateCard::setRightCornerButtonText(const std::string &rightCornerButtonText) {
    rightCornerButtonTextString = rightCornerButtonText;
    rightCornerButton->setText(rightCornerButtonText);
}