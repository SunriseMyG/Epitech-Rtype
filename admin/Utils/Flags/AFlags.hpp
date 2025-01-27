#ifndef AFLAGS_HPP
#define AFLAGS_HPP

#include <SFML/Graphics.hpp>

class AFlags {
    protected:
        sf::Vector2f position;
        sf::Vector2f scale;

    public:
        AFlags(const sf::Vector2f& position, const sf::Vector2f& scale)
            : position(position), scale(scale) {}
        virtual ~AFlags() = default;
        virtual void draw(sf::RenderWindow& window) = 0;
};

#endif