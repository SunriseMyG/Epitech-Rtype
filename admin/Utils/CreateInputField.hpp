#ifndef CREATEINPUTFIELD_HPP
    #define CREATEINPUTFIELD_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "CreateText.hpp"

    class CreateInputField {
        public:
            CreateInputField(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Font& font, const std::string& placeholder);
            void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
            void draw(sf::RenderWindow& window);
            std::string getText();
            
            void setPlaceholder(const std::string& placeholder) {
                this->placeholder = placeholder; 
                text.setText(userInput.empty() ? placeholder : userInput);
                text.setColor(userInput.empty() ? sf::Color(150, 150, 150) : sf::Color::Black);
            }
            
            void setPosition(const sf::Vector2f& newPosition) {
                rectangle.setPosition(newPosition);
                text.setPosition(sf::Vector2f(newPosition.x + 5, newPosition.y + 5));
            }
            
            void setSize(const sf::Vector2f& newSize) {
                rectangle.setSize(newSize);
                sf::Vector2f pos = rectangle.getPosition();
                text.setPosition(sf::Vector2f(pos.x + 5, pos.y + 5));
            }

            sf::Vector2f getSize() const {
                return rectangle.getSize();
            }

        private:
            sf::RectangleShape rectangle;
            CreateText text;
            std::string userInput;
            std::string placeholder;
            bool isActive;
            sf::Clock clock;
            sf::Time lastInputTime;
            sf::RectangleShape cursor;
            sf::Clock cursorClock;
            bool showCursor;
    };

#endif // CREATEINPUTFIELD_HPP