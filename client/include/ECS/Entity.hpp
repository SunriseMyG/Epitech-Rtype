/*
** EPITECH PROJECT, 2025
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Entity
*/

#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include "Component.hpp"

class Entity {
private:
    int id = -1;
    int netId = -1;
    bool hasValidId = false;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
    bool markForDeletion = false;

public:
    void setId(int newId) {
        id = newId;
        hasValidId = true;
    }

    void setNetId(int newNetId) {
        netId = newNetId;
    }
    
    bool hasId() const {
        return hasValidId;
    }
    
    int getId() const {
        if (!hasValidId) {
            return -1;
        }
        return id;
    }

    int getNetId() const {
        return netId;
    }

    void markForDelete() { markForDeletion = true; }
    bool isMarkedForDeletion() const { return markForDeletion; }

    template<typename T>
    void addComponent(T component) {
        components[std::type_index(typeid(T))] = std::make_shared<T>(component);
    }

    template<typename T, typename... Args>
    void addComponent(Args&&... args) {
        components[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    bool hasComponent() const {
        return components.find(std::type_index(typeid(T))) != components.end();
    }

    template<typename T>
    std::shared_ptr<T> getComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        return nullptr;
    }
};