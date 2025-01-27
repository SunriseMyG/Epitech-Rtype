#ifndef SPANISHFLAG_HPP
    #define SPANISHFLAG_HPP

    #include "AFlags.hpp"

    class SpanishFlag : public AFlags {
        public:
            SpanishFlag(const sf::Vector2f& position, const sf::Vector2f& scale)
                : AFlags(position, scale) {}
            
            void draw(sf::RenderWindow& window) override {
                // Bande rouge (haut)
                sf::RectangleShape redTop(sf::Vector2f(scale.x, scale.y * 0.25f));
                redTop.setPosition(position);
                redTop.setFillColor(sf::Color::Red);

                // Bande jaune (milieu)
                sf::RectangleShape yellow(sf::Vector2f(scale.x, scale.y * 0.50f));
                yellow.setPosition(position.x, position.y + scale.y * 0.25f);
                yellow.setFillColor(sf::Color(255, 204, 0)); // Or/Jaune

                // Bande rouge (bas)
                sf::RectangleShape redBottom(sf::Vector2f(scale.x, scale.y * 0.25f));
                redBottom.setPosition(position.x, position.y + scale.y * 0.75f);
                redBottom.setFillColor(sf::Color::Red);

                // Dessin des bandes
                window.draw(redTop);
                window.draw(yellow);
                window.draw(redBottom);
            }
    };

#endif // SPANISHFLAG_HPP
