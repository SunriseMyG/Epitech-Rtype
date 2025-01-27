#include "Flagsi18n.hpp"

Flagsi18n::Flagsi18n(i18n& i18n, const sf::Vector2f& position, const sf::Vector2f& scale)
    : i18n_(i18n), position(position), scale(scale) {
}

bool Flagsi18n::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePixelPos);
        
        if (position.x < worldPos.x && worldPos.x < position.x + scale.x &&
            position.y < worldPos.y && worldPos.y < position.y + scale.y) {
            
            int currentLang = static_cast<int>(i18n_.getLanguage());
            int nextLang = (currentLang + 1) % i18n_.getNumberOfLanguages();
            i18n_.setLanguage(static_cast<i18n::Language>(nextLang));
        }

        return true;
    }

    return false;
}

void Flagsi18n::draw(sf::RenderWindow& window) {
    std::shared_ptr<AFlags> flag;
    switch (i18n_.getLanguage()) {
        case i18n::Language::EN:
            flag = std::make_shared<EnglishFlag>(position, scale);
            break;
        case i18n::Language::FR:
            flag = std::make_shared<FrenchFlag>(position, scale);
            break;
        case i18n::Language::ES:
            flag = std::make_shared<SpanishFlag>(position, scale);
            break;
    }

    flag->draw(window);
}