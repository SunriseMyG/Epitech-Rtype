#ifndef FRENCHFLAG_HPP
    #define FRENCHFLAG_HPP

    #include "AFlags.hpp"

    class FrenchFlag : public AFlags {
        public:
            FrenchFlag(const sf::Vector2f& position, const sf::Vector2f& scale)
                : AFlags(position, scale) {}
            
            void draw(sf::RenderWindow& window) override {
                // Création des trois bandes : bleu, blanc, rouge
                sf::RectangleShape blue(sf::Vector2f(scale.x / 3, scale.y));
                blue.setPosition(position);
                blue.setFillColor(sf::Color::Blue);

                sf::RectangleShape white(sf::Vector2f(scale.x / 3, scale.y));
                white.setPosition(position.x + scale.x / 3, position.y);
                white.setFillColor(sf::Color::White);

                sf::RectangleShape red(sf::Vector2f(scale.x / 3, scale.y));
                red.setPosition(position.x + 2 * scale.x / 3, position.y);
                red.setFillColor(sf::Color::Red);

                // Dessin sur la fenêtre
                window.draw(blue);
                window.draw(white);
                window.draw(red);
            }
    };

#endif // FRENCHFLAG_HPP
