/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** TextureManager
*/

#include <unordered_map>
#include "TextureManager.hpp"

std::unordered_map<std::string, std::shared_ptr<sf::Texture>> TextureManager::textures;