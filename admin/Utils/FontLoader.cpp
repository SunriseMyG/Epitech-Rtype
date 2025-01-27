#include "FontLoader.hpp"
#include <iostream>

FontLoader& FontLoader::getInstance() {
    static FontLoader instance;
    return instance;
}

const sf::Font& FontLoader::getFont(const std::string& fontName, const std::string& filePath) {
    auto it = fonts.find(fontName);
    if (it != fonts.end()) {
        return it->second;
    }

    sf::Font font;
    if (!font.loadFromFile(filePath)) {
        std::cerr << "Erreur: Impossible de charger la police depuis " << filePath << std::endl;
        throw std::runtime_error("Erreur lors du chargement de la police.");
    }

    fonts[fontName] = std::move(font);
    return fonts[fontName];
}
