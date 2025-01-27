#ifndef CREATEBUTTON_HPP_
    #define CREATEBUTTON_HPP_

    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <string>
    #include <functional>
    #include "CreateText.hpp"

    class CreateButton {
        public:
            CreateButton(const sf::Vector2f &position, const sf::Vector2f &size, const sf::Color &buttonColor, const std::string &textString, const sf::Font &font, const sf::Color &textColor = sf::Color::Black);
            ~CreateButton() = default;
            void handleEvent(const sf::RenderWindow& window, const sf::Event &event, std::function<void()> onClick);
            void draw(sf::RenderWindow& window);
            void setText(const std::string &text) { buttonText->setText(text); }

            // getPosition
            sf::Vector2f getPosition() const { return position; }
            // getSize
            sf::Vector2f getSize() const { return size; }

        private:
            sf::RectangleShape button;
            CreateText* buttonText;
            sf::Font font;
            sf::Vector2f position;
            sf::Vector2f size;
            std::string textString;
            bool isButtonPressed;
    };

#endif /* !CREATEBUTTON_HPP_ */