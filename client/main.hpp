/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** main
*/

#ifndef MAIN_HPP_
#define MAIN_HPP_

#include <SFML/Graphics.hpp>
#include "./include/ECS/World.hpp"
#include "./include/ECS/Entity.hpp"
#include "./include/ECS/Component.hpp"
#include "./include/ECS/System.hpp"
#include "./include/ECS/TextureManager.hpp"
#include "./include/ECS/BossFactory.hpp"
#include "./include/Network/NetworkManager.hpp"
#include "./include/Menu/Menu.hpp"
#include <memory>

class Game {
public:
    Game(const std::string& ip, const std::string& port);
    ~Game();
    
    void run();

private:
    void initializeSystems();
    void gameLoop();

    sf::RenderWindow window;
    std::unique_ptr<World> world;
    std::unique_ptr<NetworkManager> networkManager;
    std::unique_ptr<Menu> menu;
    bool inMenu;
};

#endif /* !MAIN_HPP_ */