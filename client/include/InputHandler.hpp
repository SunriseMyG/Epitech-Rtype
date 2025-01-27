/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** InputHandler
*/

#pragma once
#include "ECS/World.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"
#include "ECS/SoundManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class InputHandler {
public:
    static void handleInput(World& world, sf::Event event);
    static void handleKeyReleased(World& world, sf::Event event);
private:
    static void handleKeyPressed(sf::Event event, std::shared_ptr<PlayerComponent> player,
        std::shared_ptr<VelocityComponent> vel,
        std::shared_ptr<PositionComponent> pos,
        World& world);
};