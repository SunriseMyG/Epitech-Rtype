/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** ParallaxBackground
*/

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>

class ParallaxBackground {
private:
    struct Layer {
        sf::Sprite sprite;
        float speed;
        float position;
    };

    struct Star {
        sf::CircleShape shape;
        float speed;
        float x, y;
    };
    
    sf::RenderWindow& window;
    std::vector<Layer> layers;
    std::vector<Star> stars;
    std::shared_ptr<sf::Texture> bgTexture;

public:
    ParallaxBackground(sf::RenderWindow& window) : window(window) {
        bgTexture = std::make_shared<sf::Texture>();
        if (!bgTexture->loadFromFile("client/sprites/bg-stage1.jpg")) {
            throw std::runtime_error("Impossible de charger la texture du background");
        }
        
        // Activer la répétition de la texture
        bgTexture->setRepeated(true);
        
        // Créer les couches avec des vitesses différentes
        createLayer(1.0f);    // Couche arrière (lente)
        createLayer(1.5f);    // Couche du milieu
        createLayer(2.0f);    // Couche avant (rapide)
        
        initializeStars();
    }

    void update() {
        float windowWidth = window.getSize().x;
        
        // Mettre à jour chaque couche
        for (auto& layer : layers) {
            layer.position -= layer.speed;
            float spriteWidth = layer.sprite.getGlobalBounds().width;
            
            // Réinitialiser la position quand une répétition complète est faite
            if (layer.position <= -spriteWidth) {
                layer.position += spriteWidth;
            }
            layer.sprite.setPosition(layer.position, layer.sprite.getPosition().y);
        }
        
        // Mettre à jour les étoiles
        for (auto& star : stars) {
            star.x -= star.speed * 2.5f;
            if (star.x < 0) {
                star.x = windowWidth;
            }
            star.shape.setPosition(star.x, star.y);
        }
    }

    void render() {
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;
        
        // Dessiner les couches de fond
        for (const auto& layer : layers) {
            float spriteWidth = layer.sprite.getGlobalBounds().width;
            float spriteHeight = layer.sprite.getGlobalBounds().height;
            
            // Calculer le nombre de répétitions nécessaires en X et Y
            int repeatX = static_cast<int>(std::ceil(windowWidth / spriteWidth)) + 2;
            int repeatY = static_cast<int>(std::ceil(windowHeight / spriteHeight)) + 1;
            
            // Dessiner la grille de sprites
            for (int y = 0; y < repeatY; y++) {
                for (int x = -1; x < repeatX; x++) {
                    sf::Sprite tileSprite = layer.sprite;
                    float posX = layer.position + (x * spriteWidth);
                    float posY = y * spriteHeight;
                    tileSprite.setPosition(posX, posY);
                    window.draw(tileSprite);
                }
            }
        }
        
        // Dessiner les étoiles
        for (const auto& star : stars) {
            window.draw(star.shape);
        }
    }

    void handleResize(const sf::Vector2u& newSize) {
        // Redimensionner chaque couche pour couvrir toute la fenêtre
        for (auto& layer : layers) {
            // Calculer l'échelle de base
            float scaleX = static_cast<float>(newSize.x) / bgTexture->getSize().x;
            float scaleY = static_cast<float>(newSize.y) / bgTexture->getSize().y;
            
            // Utiliser une échelle qui permet la répétition propre
            float scale = std::min(scaleX, scaleY);
            layer.sprite.setScale(scale, scale);
            
            // Pas besoin de centrer verticalement car on va répéter
            layer.sprite.setPosition(layer.position, 0);
        }
        
        // Réinitialiser les étoiles pour qu'elles couvrent tout l'écran
        stars.clear();
        initializeStars();
    }

private:
    void createLayer(float speed) {
        Layer layer;
        layer.sprite.setTexture(*bgTexture);
        
        // Calculer l'échelle initiale
        float scaleX = static_cast<float>(window.getSize().x) / bgTexture->getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / bgTexture->getSize().y;
        float scale = std::min(scaleX, scaleY);
        
        layer.sprite.setScale(scale, scale);
        layer.sprite.setPosition(0, 0);
        
        // Ajuster l'opacité en fonction de la vitesse
        sf::Color layerColor = sf::Color::White;
        layerColor.a = static_cast<sf::Uint8>(255 * (speed / 2.0f));
        layer.sprite.setColor(layerColor);
        
        layer.speed = speed;
        layer.position = 0;
        layers.push_back(layer);
    }

    void initializeStars() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> xDist(0.0f, window.getSize().x);
        std::uniform_real_distribution<float> yDist(0.0f, window.getSize().y * 2);
        std::uniform_real_distribution<float> speedDist(1.0f, 4.0f);
        std::uniform_real_distribution<float> sizeDist(1.0f, 3.0f);

        for (int i = 0; i < 100; ++i) {
            Star star;
            star.x = xDist(gen);
            star.y = yDist(gen);
            star.speed = speedDist(gen);
            
            star.shape.setRadius(sizeDist(gen));
            star.shape.setFillColor(sf::Color(255, 255, 255, 
                static_cast<sf::Uint8>(100 + (star.speed / 4.0f) * 155)));
            star.shape.setPosition(star.x, star.y);
            
            stars.push_back(star);
        }
    }
};
