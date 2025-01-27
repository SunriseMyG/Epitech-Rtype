
#ifndef CREATECARDSTAT_HPP_
    #define CREATECARDSTAT_HPP_

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <string>
    #include <functional>
    #include "CreateText.hpp"
    #include "CreateButton.hpp"

    class CreateStatCard {
        public:
            CreateStatCard(const sf::Vector2f &position,
                const sf::Vector2f &size,
                const sf::Color &cardColor,
                const sf::Font &font,
                const std::string &leftCornerButtonText,
                const std::string &rightCornerButtonText,
                const std::string id = "-1",
                const std::string nbrDeaths = nullptr,
                const std::string nbrGames = nullptr,
                const std::string nbrKills = nullptr,
                const std::string nbrLosses = nullptr,
                const std::string nbrWins = nullptr,
                const std::string score = nullptr);
            ~CreateStatCard() = default;
            void handleEventKick(const sf::RenderWindow &window, const sf::Event &event, std::function<void()> onClick);
            void handleEventBan(const sf::RenderWindow &window, const sf::Event &event, std::function<void()> onClick);
            void draw(sf::RenderWindow &window);
            int getPlayerId() const { return std::stoi(id); }

        private:

            sf::RectangleShape card;
            CreateText* idText;
            CreateText* nbrDeathsText;
            CreateText* nbrGamesText;
            CreateText* nbrKillsText;
            CreateText* nbrLossesText;
            CreateText* nbrWinsText;
            CreateText* scoreText;
            CreateButton* leftCornerButton;
            CreateButton* rightCornerButton;
            sf::Font font;
            sf::Vector2f position;
            sf::Vector2f size;
            std::string id;
            std::string nbrDeaths;
            std::string nbrGames;
            std::string nbrKills;
            std::string nbrLosses;
            std::string nbrWins;
            std::string score;
            std::string leftCornerButtonText;
            std::string rightCornerButtonText;

    };

#endif /* !CREATECARDSTAT_HPP_ */