/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** SoundManager
*/

#pragma once
#include <SFML/Audio.hpp>
#include <memory>
#include <unordered_map>
#include <string>

class SoundManager {
private:
    static std::unordered_map<std::string, std::shared_ptr<sf::SoundBuffer>> soundBuffers;
    static std::vector<std::shared_ptr<sf::Sound>> activeSounds;
    static float globalVolume;

public:
    static std::shared_ptr<sf::Sound> playSound(const std::string& filename);
    
    static void setGlobalVolume(float volume) {
        globalVolume = volume;
        for (auto& sound : activeSounds) {
            if (sound && sound->getStatus() != sf::Sound::Stopped) {
                sound->setVolume(volume);
            }
        }
    }
    
    static float getGlobalVolume() {
        return globalVolume;
    }
};
