/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** TextureManager
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <memory>

class TextureManager {
private:
    static std::unordered_map<std::string, std::shared_ptr<sf::Texture>> textures;

public:
    static std::shared_ptr<sf::Texture> loadTexture(const std::string& path) {
        if (textures.find(path) != textures.end()) {
            return textures[path];
        }

        auto texture = std::make_shared<sf::Texture>();
        if (!texture->loadFromFile(path)) {
            return nullptr;
        }

        textures[path] = texture;
        return texture;
    }
};