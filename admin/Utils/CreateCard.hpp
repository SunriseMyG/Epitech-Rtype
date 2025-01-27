#ifndef CREATECARD_HPP_
    #define CREATECARD_HPP_

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <string>
    #include <functional>
    #include "CreateText.hpp"
    #include "CreateButton.hpp"

    class CreateCard {
        public:
            CreateCard(const sf::Vector2f &position,
                const sf::Vector2f &size,
                const sf::Color &cardColor,
                const sf::Font &font,
                const std::string &leftCornerText,
                const std::string &rightCornerText,
                const std::string &rightCornerButtonText,
                const std::string id = "-1",
                const std::string roomName = nullptr,
                const std::string roomPassword = nullptr,
                const std::string roomState = nullptr,
                const int maxPlayers = 0,
                const int currentPlayers = 0);
            ~CreateCard() = default;
            void handleEvent(const sf::RenderWindow& window, const sf::Event &event, std::function<void()> onClick);
            void draw(sf::RenderWindow& window);
            void setRightCornerButtonText(const std::string &rightCornerButtonText);
            CreateButton* rightCornerButton;
            std::string id;
            std::string roomName;
            std::string roomPassword;
            std::string roomState;
            int maxPlayers;
            int currentPlayers;

        private:
            sf::RectangleShape card;
            CreateText* leftCornerText;
            CreateText* rightCornerText;
            sf::Font font;
            sf::Vector2f position;
            sf::Vector2f size;
            std::string leftCornerTextString;
            std::string rightCornerTextString;
            std::string rightCornerButtonTextString;
    };
    

#endif /* !CREATECARD_HPP_ */