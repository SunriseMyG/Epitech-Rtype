#ifndef FONT_LOADER_HPP
#define FONT_LOADER_HPP

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class FontLoader {
public:
    static FontLoader& getInstance();

    const sf::Font& getFont(const std::string& fontName, const std::string& filePath);

private:
    FontLoader() = default;
    ~FontLoader() = default;

    FontLoader(const FontLoader&) = delete;
    FontLoader& operator=(const FontLoader&) = delete;

    std::map<std::string, sf::Font> fonts;
};

#endif // FONT_LOADER_HPP
