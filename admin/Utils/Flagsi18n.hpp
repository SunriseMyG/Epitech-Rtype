#ifndef FLAGSI18N_HPP
    #define FLAGSI18N_HPP

    #include "../I18n/i18n.hpp"
    #include <SFML/Graphics.hpp>

    #include "./Flags/EnglishFlag.hpp"
    #include "./Flags/FrenchFlag.hpp"
    #include "./Flags/SpanishFlag.hpp"

    class Flagsi18n {
        public:
            Flagsi18n(i18n& i18n, const sf::Vector2f& position, const sf::Vector2f& scale);
            bool handleEvent(const sf::Event& event, sf::RenderWindow& window);
            void draw(sf::RenderWindow& window);

        private:
            i18n& i18n_;
            sf::Vector2f position;
            sf::Vector2f scale;
    };

#endif // FLAGSI18N_HPP