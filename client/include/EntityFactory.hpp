/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** EntityFactory
*/

#pragma once
#include "ECS/World.hpp"
#include "ECS/Entity.hpp"
#include "ECS/Component.hpp"
#include "ECS/TextureManager.hpp"

class EntityFactory {
public:
    static void createPlayer(World& world, float x, float y, int playerId, int netId);
    static void createBullet(World& world, float x, float y);
    static void createSingleBullet(World& world, float x, float y, float angleOffset);
    static void createBoss(World& world);
    static void createPowerup(World& world, float x, float y, int id);
    static std::shared_ptr<Entity> createEnemyBullet(World& world, float x, float y);

};