/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** SoundManager
*/

#include "../include/ECS/SoundManager.hpp"

std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> SoundManager::soundBuffers;
std::vector<std::shared_ptr<sf::Sound>> SoundManager::activeSounds;
float SoundManager::globalVolume = 100.0f;

std::shared_ptr<sf::Sound> SoundManager::playSound(const std::string& filename) {
    if (soundBuffers.find(filename) == soundBuffers.end()) {
        auto buffer = std::make_shared<sf::SoundBuffer>();
        if (!buffer->loadFromFile(filename)) {
            return nullptr;
        }
        soundBuffers[filename] = buffer;
    }

    auto sound = std::make_shared<sf::Sound>();
    sound->setBuffer(*soundBuffers[filename]);
    sound->setVolume(globalVolume);
    sound->play();
    activeSounds.push_back(sound);

    activeSounds.erase(
        std::remove_if(activeSounds.begin(), activeSounds.end(),
            [](const auto& sound) { return sound->getStatus() == sf::Sound::Stopped; }),
        activeSounds.end());

    return sound;
}
