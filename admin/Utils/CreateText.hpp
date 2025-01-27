#ifndef CREATETEXT_HPP
    #define CREATETEXT_HPP

    #include <SFML/Graphics.hpp>
    #include <string>

    class CreateText {
        public:
            CreateText(const sf::Font& font, const std::string& text, unsigned int characterSize, const sf::Color& color, const sf::Vector2f& position);

            void setText(const std::string& text);
            void setPosition(const sf::Vector2f& position);
            void setColor(const sf::Color& color);
            void setCharacterSize(unsigned int size);

            sf::Vector2f getPosition() const {
                return text.getPosition();
            }

            void draw(sf::RenderWindow& window) const;
            sf::FloatRect getGlobalBounds() const;

        private:
            sf::Text text;
    };

#endif // CREATETEXT_HPP