/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** main
*/
#include "./main.hpp"
#include <iostream>
#include <SFML/Window/WindowStyle.hpp>

Game::Game(const std::string& ip, const std::string& port) 
    : window(sf::VideoMode(800, 600), "R-Type"),
      world(std::make_unique<World>(window)),
      networkManager(std::make_unique<NetworkManager>(ip, port)),
      inMenu(true)
{
    menu = std::make_unique<Menu>(window, world.get(), ip, port);
    
    menu->setStateChangeCallback([this](MenuState state) {
        if (state == MenuState::GAME) {
            inMenu = false;
        } else {
            inMenu = true;
        }
    });

    Menu::globalStateChangeCallback = [this](MenuState state) {
        menu->setState(state);
    };
}

Game::~Game() = default;

void Game::initializeSystems() {
    world->addSystem<MovementSystem>();
    world->addSystem<PowerUpSystem>();
    world->addSystem<BulletSystem>();
    world->addSystem<EnemySystem>();
    world->addSystem<CollisionSystem>(*networkManager);
    world->addSystem<RenderSystem>(window);
    world->addSystem<BossSystem>();
    world->addSystem<ScoreRenderSystem>(window);
    world->addSystem<ExplosionSystem>();
    world->addSystem<MovementPatternSystem>();
    world->addSystem<ObstacleSystem>();
}

void Game::gameLoop() {
    sf::Clock clock;
    float accumulator = 0.0f;
    const float dt = 1.0f / 60.0f;
    
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    window.setActive(false);
    
    while (window.isOpen()) {
        float frameTime = clock.restart().asSeconds();
        accumulator += frameTime;

        while (accumulator >= dt) {
            world->update(dt);
            accumulator -= dt;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                networkManager->disconnect();
                window.close();
            }
            
            menu->handleEvent(event);
            
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
                if (inMenu) {
                    menu->handleResize();
                }
            }
        }

        if (!inMenu) {
            menu->handleGameEvents();
        }

        window.clear();
        if (inMenu) {
            menu->render();
        } else {
            world->render();
        }
        window.display();
    }
}

void Game::run() {
    try {
        networkManager->connect();
        initializeSystems();
        gameLoop();
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        throw;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip_address> <port>" << std::endl;
        return 84;
    }

    try {
        Game game(argv[1], argv[2]);
        game.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Failed to run game: " << e.what() << std::endl;
        return 84;
    }
}