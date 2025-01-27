/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** World
*/

#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "System.hpp"
#include <SFML/Graphics.hpp>
#include "ParallaxBackground.hpp"

class World {
private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::unique_ptr<System>> systems;
    std::unique_ptr<ParallaxBackground> background;
    sf::RenderWindow& window;

public:
    World(sf::RenderWindow& window) : window(window) {
        background = std::make_unique<ParallaxBackground>(window);
        addSystem<MovementPatternSystem>();
    }

    void addEntity(std::shared_ptr<Entity> entity) {
        entities.push_back(entity);
    }

    void removeEntity(std::shared_ptr<Entity> entity) {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    template<typename T, typename... Args>
    void addSystem(Args&&... args) {
        systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    void render() {
        background->render();
        for (auto& system : systems) {
            if (auto renderSystem = dynamic_cast<RenderSystem*>(system.get())) {
                renderSystem->update(entities, 0.0f);
            }
            if (auto scoreSystem = dynamic_cast<ScoreRenderSystem*>(system.get())) {
                scoreSystem->update(entities, 0.0f);
            }
        }
    }

    void update(float deltaTime = 0.0f) {
        background->update();
        for (auto& system : systems) {
            system->update(entities, deltaTime);
        }
    }

    const std::vector<std::shared_ptr<Entity>>& getEntities() const {
        return entities;
    }

    void reset() {
        entities.clear();
    }

    template<typename T>
    std::shared_ptr<Entity> findEntityWithComponent() {
        for (auto& entity : entities) {
            if (entity->hasComponent<T>()) {
                return entity;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Entity> findEntityById(int id) {
        for (const auto& entity : entities) {
            if (entity && entity->getId() == id) {
                return entity;
            }
        }
        return nullptr;
    }
};