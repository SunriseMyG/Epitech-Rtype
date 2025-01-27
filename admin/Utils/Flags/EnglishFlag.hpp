#ifndef ENGLISHFLAG_HPP
    #define ENGLISHFLAG_HPP

    #include "AFlags.hpp"

    class EnglishFlag : public AFlags {
        public:
            EnglishFlag(const sf::Vector2f& position, const sf::Vector2f& scale)
                : AFlags(position, scale) {}
            
            void draw(sf::RenderWindow& window) override {
                // Fond blanc
                sf::RectangleShape background(scale);
                background.setPosition(position);
                background.setFillColor(sf::Color::White);

                // Bande verticale rouge
                sf::RectangleShape verticalCross(sf::Vector2f(scale.x * 0.2f, scale.y));
                verticalCross.setPosition(position.x + scale.x * 0.4f, position.y);
                verticalCross.setFillColor(sf::Color::Red);

                // Bande horizontale rouge
                sf::RectangleShape horizontalCross(sf::Vector2f(scale.x, scale.y * 0.2f));
                horizontalCross.setPosition(position.x, position.y + scale.y * 0.4f);
                horizontalCross.setFillColor(sf::Color::Red);

                // Dessin des formes
                window.draw(background);
                window.draw(verticalCross);
                window.draw(horizontalCross);
            }
    };

#endif // ENGLISHFLAG_HPP
