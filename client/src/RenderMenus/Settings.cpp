/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Menu
*/

#include "../../include/Menu/Menu.hpp"
#include "../../include/Parser/jsonParser.hpp"
#include <iostream>
#include <algorithm>
#include <thread>

void Menu::renderSettingsMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    sf::RectangleShape background;
    background.setSize(sf::Vector2f(windowWidth, windowHeight));
    background.setFillColor(sf::Color(0, 0, 0, 200));
    window.draw(background);

    sf::Text settingsTitle;
    settingsTitle.setFont(isHighContrastMode ? robotoFont : font);
    settingsTitle.setString("Parametres");
    settingsTitle.setCharacterSize(window.getSize().y * 0.04f);
    settingsTitle.setPosition(window.getSize().x * 0.5f - settingsTitle.getLocalBounds().width / 2, 
                            window.getSize().y * 0.1f);
    settingsTitle.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
    window.draw(settingsTitle);

    float startY = window.getSize().y * 0.3f;
    float spacing = window.getSize().y * 0.08f;
    
    std::vector<std::pair<std::string, sf::Keyboard::Key>> bindings = {
        {"Haut", keyBindings.moveUp},
        {"Bas", keyBindings.moveDown},
        {"Gauche", keyBindings.moveLeft},
        {"Droite", keyBindings.moveRight},
        {"Tirer", keyBindings.shoot}
    };

    for (size_t i = 0; i < bindings.size(); i++) {
        sf::Text label;
        label.setFont(isHighContrastMode ? robotoFont : font);
        label.setString(bindings[i].first);
        label.setCharacterSize(window.getSize().y * 0.03f);
        label.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
        label.setPosition(window.getSize().x * 0.3f, startY + (i * spacing));
        window.draw(label);
        sf::RectangleShape keyButton(sf::Vector2f(window.getSize().x * 0.2f, window.getSize().y * 0.05f));
        keyButton.setPosition(window.getSize().x * 0.5f, startY + (i * spacing));
        keyButton.setFillColor(isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(70, 70, 70));
        window.draw(keyButton);
        sf::Text keyText;
        keyText.setFont(isHighContrastMode ? robotoFont : font);
        keyText.setString(isWaitingForKey && currentKeyBinding == bindings[i].first ? 
                         "Appuyez sur une touche..." : getKeyName(bindings[i].second));
        keyText.setCharacterSize(window.getSize().y * 0.03f);
        keyText.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
        sf::FloatRect textBounds = keyText.getLocalBounds();
        keyText.setPosition(
            keyButton.getPosition().x + (keyButton.getSize().x - textBounds.width) / 2,
            keyButton.getPosition().y + (keyButton.getSize().y - textBounds.height) / 2
        );
        window.draw(keyText);
    }
    daltonianButton.setFillColor(isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(70, 70, 70));
    window.draw(daltonianButton);
    daltonianText.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
    window.draw(daltonianText);
    volumeLabel.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
    window.draw(volumeLabel);
    volumeSlider.setFillColor(isDaltonianMode ? sf::Color(0, 100, 180) : sf::Color(70, 70, 70));
    window.draw(volumeSlider);
    volumeSliderHandle.setFillColor(isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(150, 150, 150));
    window.draw(volumeSliderHandle);
    if (isDaltonianMode) {
        accessibilityButton.setFillColor(sf::Color(0, 170, 230));  // Bleu clair pour le mode daltonien
        accessibilityText.setFillColor(sf::Color::Yellow);
    } else {
        accessibilityButton.setFillColor(isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70));
        accessibilityText.setFillColor(isHighContrastMode ? sf::Color::Black : sf::Color::White);
    }

    if (isHighContrastMode) {
        accessibilityButton.setOutlineColor(sf::Color::Black);
        accessibilityButton.setOutlineThickness(2.0f);
    } else {
        accessibilityButton.setOutlineThickness(0.0f);
    }

    window.draw(accessibilityButton);
    accessibilityText.setFont(isHighContrastMode ? robotoFont : font);
    accessibilityText.setCharacterSize(windowHeight * 0.03f);
    centerTextInButton(accessibilityText, accessibilityButton);
    window.draw(accessibilityText);

    updateVolumeSliderPositions();
}

void Menu::handleSettingsEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseButtonPressed) {
        if (daltonianButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isDaltonianMode = !isDaltonianMode;
            updateDaltonianMode();
            applyDaltonianColors();
            
            // Mise à jour des couleurs des boutons de touches
            float windowWidth = window.getSize().x;
            float windowHeight = window.getSize().y;
            float startY = windowHeight * 0.3f;
            float spacing = windowHeight * 0.08f;
            
            for (int i = 0; i < 5; i++) {
                sf::RectangleShape keyButton(sf::Vector2f(windowWidth * 0.2f, windowHeight * 0.05f));
                keyButton.setPosition(windowWidth * 0.5f, startY + (i * spacing));
                
                if (isDaltonianMode) {
                    keyButton.setFillColor(sf::Color(0, 170, 230));  // Bleu clair pour le mode daltonien
                } else {
                    keyButton.setFillColor(sf::Color(70, 70, 70));   // Couleur normale
                }
            }
        }
    }

       if (event.type == sf::Event::MouseMoved) {
        // Effet de survol pour le bouton d'accessibilité
        if (accessibilityButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            accessibilityButton.setFillColor(sf::Color(120, 120, 120));
        } else {
            accessibilityButton.setFillColor(sf::Color(100, 100, 100));
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        // Ajout de la gestion du clic sur le bouton d'accessibilité
        if (accessibilityButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isHighContrastMode = !isHighContrastMode;
            updateAccessibilityStyles();
            updateDaltonianMode();
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (isWaitingForKey) {
            isWaitingForKey = false;
            currentKeyBinding = "";
        } else {
            isSettingsMenuOpen = false;
        }
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        if (!isSettingsMenuOpen) {
            if (settingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                isSettingsMenuOpen = true;
            }
        } else {
            
            // Gestion des clics sur les boutons de touches
            float windowWidth = window.getSize().x;
            float windowHeight = window.getSize().y;
            float startY = windowHeight * 0.3f;
            float spacing = windowHeight * 0.08f;
            
            for (int i = 0; i < 5; i++) {
                sf::RectangleShape keyButton(sf::Vector2f(windowWidth * 0.2f, windowHeight * 0.05f));
                keyButton.setPosition(windowWidth * 0.5f, startY + (i * spacing));
                
                if (keyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isWaitingForKey = true;
                    switch (i) {
                        case 0: currentKeyBinding = "Haut"; break;
                        case 1: currentKeyBinding = "Bas"; break;
                        case 2: currentKeyBinding = "Gauche"; break;
                        case 3: currentKeyBinding = "Droite"; break;
                        case 4: currentKeyBinding = "Tirer"; break;
                    }
                }
            }
        }
    } else if (event.type == sf::Event::KeyPressed && isWaitingForKey) {
        // Mise à jour de la touche
        if (currentKeyBinding == "Haut") keyBindings.moveUp = event.key.code;
        else if (currentKeyBinding == "Bas") keyBindings.moveDown = event.key.code;
        else if (currentKeyBinding == "Gauche") keyBindings.moveLeft = event.key.code;
        else if (currentKeyBinding == "Droite") keyBindings.moveRight = event.key.code;
        else if (currentKeyBinding == "Tirer") keyBindings.shoot = event.key.code;
        
        isWaitingForKey = false;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect sliderBounds = volumeSlider.getGlobalBounds();
        sf::FloatRect handleBounds = volumeSliderHandle.getGlobalBounds();
        
        if (handleBounds.contains(mousePos.x, mousePos.y) || sliderBounds.contains(mousePos.x, mousePos.y)) {
            isDraggingVolume = true;
            float relativeX = mousePos.x - volumeSlider.getPosition().x;
            currentVolume = std::clamp(relativeX / volumeSlider.getSize().x, 0.0f, 1.0f);
            updateVolumeSliderPositions();
            SoundManager::setGlobalVolume(currentVolume * 100.0f);
        }
    } else if (event.type == sf::Event::MouseButtonReleased) {
        isDraggingVolume = false;
    } else if (event.type == sf::Event::MouseMoved && isDraggingVolume) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        float relativeX = mousePos.x - volumeSlider.getPosition().x;
        currentVolume = std::clamp(relativeX / volumeSlider.getSize().x, 0.0f, 1.0f);
        updateVolumeSliderPositions();
        SoundManager::setGlobalVolume(currentVolume * 100.0f);
    }
}


void Menu::updateVolumeHandlePosition() {
    float handleX = volumeSlider.getPosition().x + (volumeSlider.getSize().x * currentVolume);
    float handleY = volumeSlider.getPosition().y + (volumeSlider.getSize().y / 2);
    volumeSliderHandle.setPosition(handleX, handleY);
}

void Menu::updateVolumeSliderPositions() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Position du slider
    float sliderWidth = windowWidth * 0.2f;
    float sliderHeight = windowHeight * 0.01f;
    float sliderX = windowWidth * 0.5f;
    float sliderY = windowHeight * 0.8f;

    volumeSlider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    volumeSlider.setPosition(sliderX, sliderY);
    
    // Position de la poignée
    float handleSize = windowHeight * 0.02f;
    volumeSliderHandle.setSize(sf::Vector2f(handleSize, handleSize));
    volumeSliderHandle.setOrigin(handleSize / 2, handleSize / 2);
    
    float handleX = sliderX + (sliderWidth * currentVolume);
    float handleY = sliderY + (sliderHeight / 2.0f);
    volumeSliderHandle.setPosition(handleX, handleY);
    
    // Position du label
    volumeLabel.setCharacterSize(windowHeight * 0.03f);
    volumeLabel.setPosition(windowWidth * 0.3f, windowHeight * 0.78f);
}

