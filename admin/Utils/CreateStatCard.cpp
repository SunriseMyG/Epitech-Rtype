

#include "CreateStatCard.hpp"

CreateStatCard::CreateStatCard(
    const sf::Vector2f &position,
    const sf::Vector2f &size,
    const sf::Color &cardColor,
    const sf::Font &font,
    const std::string &leftCornerButtonText,
    const std::string &rightCornerButtonText,
    const std::string id,
    const std::string nbrDeaths,
    const std::string nbrGames,
    const std::string nbrKills,
    const std::string nbrLosses,
    const std::string nbrWins,
    const std::string score)
: position(position)
, size(size)
, font(font)
, leftCornerButtonText(leftCornerButtonText)
, rightCornerButtonText(rightCornerButtonText)
, id(id)
, nbrDeaths(nbrDeaths)
, nbrGames(nbrGames)
, nbrKills(nbrKills)
, nbrLosses(nbrLosses)
, nbrWins(nbrWins)
, score(score)
{
    card.setOrigin(size.x / 2, size.y / 2);
    card.setPosition(position);
    card.setSize(size);
    card.setFillColor(sf::Color::Transparent);
    card.setOutlineThickness(2.0f);
    card.setOutlineColor(cardColor);

    float textOffsetX = position.x - (size.x / 2) + 10;
    float textOffsetY = position.y - (size.y / 2) + 10;

    this->idText = new CreateText(font, id, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY));
    this->nbrDeathsText = new CreateText(font, nbrDeaths, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 30));
    this->nbrGamesText = new CreateText(font, nbrGames, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 60));
    this->nbrKillsText = new CreateText(font, nbrKills, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 90));
    this->nbrLossesText = new CreateText(font, nbrLosses, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 120));
    this->nbrWinsText = new CreateText(font, nbrWins, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 150));
    this->scoreText = new CreateText(font, score, 24, sf::Color::White, sf::Vector2f(textOffsetX, textOffsetY + 180));

    leftCornerButton = new CreateButton(sf::Vector2f(position.x - (size.x / 2) + 75, position.y + (size.y / 2) - 25), sf::Vector2f(150, 50), sf::Color(128, 128, 128), leftCornerButtonText, font, sf::Color::White);
    rightCornerButton = new CreateButton(sf::Vector2f(position.x + (size.x / 2) - 75, position.y + (size.y / 2) - 25), sf::Vector2f(150, 50), sf::Color(128, 128, 128), rightCornerButtonText, font, sf::Color::White);
}

void CreateStatCard::handleEventKick(const sf::RenderWindow& window, const sf::Event &event, std::function<void()> onClick) {
    leftCornerButton->handleEvent(window, event, onClick);
}

void CreateStatCard::handleEventBan(const sf::RenderWindow &window, const sf::Event &event, std::function<void()> onClick) {
    rightCornerButton->handleEvent(window, event, onClick);
}

void CreateStatCard::draw(sf::RenderWindow& window) {
    window.draw(card);
    nbrDeathsText->draw(window);
    nbrGamesText->draw(window);
    nbrKillsText->draw(window);
    nbrLossesText->draw(window);
    nbrWinsText->draw(window);
    scoreText->draw(window);

    leftCornerButton->draw(window);
    rightCornerButton->draw(window);
}
