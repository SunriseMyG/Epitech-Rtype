/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** InputHandler
*/

#include "../include/InputHandler.hpp"
#include "../include/EntityFactory.hpp"
#include "../include/ECS/SoundManager.hpp"

void InputHandler::handleInput(World& world, sf::Event event) {
    auto entities = world.getEntities();
    for (auto& entity : entities) {
        if (entity->hasComponent<PlayerComponent>()) {
            auto player = entity->getComponent<PlayerComponent>();
            auto vel = entity->getComponent<VelocityComponent>();
            auto pos = entity->getComponent<PositionComponent>();

            if (event.type == sf::Event::KeyPressed) {
                handleKeyPressed(event, player, vel, pos, world);
            }
        }
    }
}

void InputHandler::handleKeyPressed(sf::Event event, std::shared_ptr<PlayerComponent> player,
        std::shared_ptr<VelocityComponent> vel,
        std::shared_ptr<PositionComponent> pos,
        World& world)
{
    switch (event.key.code) {
        case sf::Keyboard::Z:
            vel->dy = -0.013f;
            player->state = PlayerComponent::AnimationState::UP;
            SoundManager::playSound("client/sound/space-ship-102433.ogg");
            break;
        case sf::Keyboard::S:
            vel->dy = 0.013f;
            player->state = PlayerComponent::AnimationState::DOWN;
            SoundManager::playSound("client/sound/space-ship-102433.ogg");
            break;
        case sf::Keyboard::Q:
            if (pos->x > -0.95f) {
                vel->dx = -0.013f;
                SoundManager::playSound("client/sound/space-ship-102433.ogg");
            }
            break;
        case sf::Keyboard::D:
            if (pos->x < 0.95f) {
                vel->dx = 0.013f;
                SoundManager::playSound("client/sound/space-ship-102433.ogg");
            }
            break;
        case sf::Keyboard::Space:
            EntityFactory::createBullet(world, pos->x, pos->y);
            SoundManager::playSound("client/sound/blaster-103340.ogg");
            break;
    }
}

void InputHandler::handleKeyReleased(World& world, sf::Event event) {
    auto entities = world.getEntities();
    for (auto& entity : entities) {
        if (entity->hasComponent<PlayerComponent>()) {
            auto player = entity->getComponent<PlayerComponent>();
            auto vel = entity->getComponent<VelocityComponent>();

            switch (event.key.code) {
                case sf::Keyboard::Z:
                case sf::Keyboard::S:
                    vel->dy = 0.0f;
                    player->state = PlayerComponent::AnimationState::IDLE;
                    break;
                case sf::Keyboard::Q:
                case sf::Keyboard::D:
                    vel->dx = 0.0f;
                    break;
            }
        }
    }
}
