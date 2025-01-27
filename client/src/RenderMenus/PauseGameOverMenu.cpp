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

void Menu::renderPauseMenu() {
    window.draw(pauseBackground);
    window.draw(resumeButton);
    window.draw(mainMenuButton);
    window.draw(resumeText);
    window.draw(mainMenuText);
    window.draw(accessibilityButton);
    window.draw(accessibilityText);
}

void Menu::renderGameOver() {
    window.draw(gameOverBackground);
    window.draw(gameOverText);
    window.draw(restartButton);
    window.draw(mainMenuFromGameOverButton);
    window.draw(restartText);
    window.draw(mainMenuFromGameOverText);
}

void Menu::updatePauseMenuPositions() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    resumeButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    
    resumeButton.setPosition(centerX, windowHeight * 0.4f);
    mainMenuButton.setPosition(centerX, windowHeight * 0.6f);
    
    float textSize = windowHeight * 0.04f;
    resumeText.setCharacterSize(textSize);
    mainMenuText.setCharacterSize(textSize);
    
    centerTextInButton(resumeText, resumeButton);
    centerTextInButton(mainMenuText, mainMenuButton);
}

void Menu::updateGameOverPositions() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    gameOverText.setCharacterSize(windowHeight * 0.1f);
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (windowWidth - textBounds.width) / 2,
        windowHeight * 0.2f
    );
    
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    restartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuFromGameOverButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    
    restartButton.setPosition(centerX, windowHeight * 0.5f);
    mainMenuFromGameOverButton.setPosition(centerX, windowHeight * 0.65f);
    
    float textSize = windowHeight * 0.04f;
    restartText.setCharacterSize(textSize);
    mainMenuFromGameOverText.setCharacterSize(textSize);
    
    centerTextInButton(restartText, restartButton);
    centerTextInButton(mainMenuFromGameOverText, mainMenuFromGameOverButton);
}

void Menu::createWinMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Fond semi-transparent
    winBackground.setSize(sf::Vector2f(windowWidth, windowHeight));
    winBackground.setFillColor(sf::Color(0, 0, 0, 200));
    
    // Texte "PARTIE TERMINEE"
    winText.setFont(font);
    winText.setString("PARTIE TERMINEE");
    winText.setCharacterSize(windowHeight * 0.1f);
    winText.setFillColor(sf::Color::Green);
    
    // Centrer le texte
    sf::FloatRect textBounds = winText.getLocalBounds();
    winText.setPosition(
        (windowWidth - textBounds.width) / 2,
        windowHeight * 0.2f
    );
    
    // Bouton retour aux rooms
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    backToRoomsButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    backToRoomsButton.setPosition(centerX, windowHeight * 0.5f);
    backToRoomsButton.setFillColor(sf::Color(100, 100, 100));
    
    backToRoomsText.setFont(font);
    backToRoomsText.setString("Retour aux salles");
    backToRoomsText.setCharacterSize(windowHeight * 0.04f);
    sf::FloatRect backBounds = backToRoomsText.getLocalBounds();
    backToRoomsText.setPosition(
        centerX + (buttonWidth - backBounds.width) / 2,
        windowHeight * 0.5f + (buttonHeight - backBounds.height) / 2
    );
}


void Menu::renderWinScreen() {
    window.draw(winBackground);
    window.draw(winText);
    window.draw(backToRoomsButton);
    window.draw(backToRoomsText);

}

void Menu::createPauseMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Fond semi-transparent
    pauseBackground.setSize(sf::Vector2f(windowWidth, windowHeight));
    pauseBackground.setFillColor(sf::Color(0, 0, 0, 128));
    
    // Dimensions des boutons
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    // Positions verticales
    float resumeY = windowHeight * 0.4f;
    float mainMenuY = windowHeight * 0.6f;
    
    resumeButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    
    resumeButton.setPosition(window.getSize().x / 2 - buttonWidth / 2, window.getSize().y / 2 - buttonHeight / 2 + 100);
    mainMenuButton.setPosition(window.getSize().x / 2 - buttonWidth / 2, window.getSize().y / 2 - buttonHeight / 2 + 200);
    
    resumeButton.setFillColor(sf::Color(100, 100, 100));
    mainMenuButton.setFillColor(sf::Color(100, 100, 100));
    
    // Configuration du texte
    float textSize = windowHeight * 0.04f;
    
    resumeText.setFont(font);
    mainMenuText.setFont(font);
    
    resumeText.setString("Reprendre");
    mainMenuText.setString("Menu Principal");
    
    resumeText.setCharacterSize(textSize);
    mainMenuText.setCharacterSize(textSize);
    
    resumeText.setPosition(window.getSize().x / 2 - buttonWidth / 2 + 10, window.getSize().y / 2 - buttonHeight / 2 + 100);
    mainMenuText.setPosition(window.getSize().x / 2 - buttonWidth / 2 + 10, window.getSize().y / 2 - buttonHeight / 2 + 200);

    // Configuration du bouton d'accessibilité pour le menu pause
    float accessibilityButtonWidth = windowWidth * 0.12f;
    float accessibilityButtonHeight = windowHeight * 0.05f;
    
    accessibilityButton.setSize(sf::Vector2f(accessibilityButtonWidth, accessibilityButtonHeight));
    accessibilityButton.setPosition(windowWidth * 0.05f, windowHeight * 0.05f);
    accessibilityButton.setFillColor(sf::Color(100, 100, 100));
    
    accessibilityText.setFont(font);
    accessibilityText.setString("Accessibilite");
    accessibilityText.setCharacterSize(windowHeight * 0.03f);
    
    sf::FloatRect accessBounds = accessibilityText.getLocalBounds();
    accessibilityText.setPosition(
        windowWidth * 0.05f + (accessibilityButtonWidth - accessBounds.width) / 2,
        windowHeight * 0.05f + (accessibilityButtonHeight - accessBounds.height) / 2
    );

    // Configuration du slider de volume
    float sliderWidth = windowWidth * 0.2f;
    float sliderHeight = windowHeight * 0.01f;
    float sliderX = windowWidth * 0.5f;
    float sliderY = windowHeight * 0.8f;

    volumeSlider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
    volumeSlider.setPosition(sliderX, sliderY);
    volumeSlider.setFillColor(sf::Color(70, 70, 70));

    volumeSliderHandle.setSize(sf::Vector2f(windowHeight * 0.02f, windowHeight * 0.02f));
    volumeSliderHandle.setOrigin(volumeSliderHandle.getSize().x / 2, volumeSliderHandle.getSize().y / 2);
    volumeSliderHandle.setFillColor(sf::Color(150, 150, 150));

    volumeLabel.setFont(font);
    volumeLabel.setString("Volume");
    volumeLabel.setCharacterSize(windowHeight * 0.03f);
    volumeLabel.setPosition(windowWidth * 0.3f, windowHeight * 0.78f);

    currentVolume = 1.0f;
    isDraggingVolume = false;
    updateVolumeHandlePosition();
}

void Menu::createGameOverMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Fond semi-transparent
    gameOverBackground.setSize(sf::Vector2f(windowWidth, windowHeight));
    gameOverBackground.setFillColor(sf::Color(0, 0, 0, 200));
    
    // Texte "GAME OVER"
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(windowHeight * 0.1f);
    gameOverText.setFillColor(sf::Color::Red);
    
    // Centrer le texte
    sf::FloatRect textBounds = gameOverText.getLocalBounds();
    gameOverText.setPosition(
        (windowWidth - textBounds.width) / 2,
        windowHeight * 0.2f
    );
    
    // Dimensions des boutons
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    // Bouton Rejouer
    restartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    restartButton.setPosition(centerX, windowHeight * 0.5f);
    restartButton.setFillColor(sf::Color(100, 100, 100));
    
    restartText.setFont(font);
    restartText.setString("Rejouer");
    restartText.setCharacterSize(windowHeight * 0.04f);
    sf::FloatRect restartBounds = restartText.getLocalBounds();
    restartText.setPosition(
        centerX + (buttonWidth - restartBounds.width) / 2,
        windowHeight * 0.5f + (buttonHeight - restartBounds.height) / 2
    );
    
    // Bouton Menu Principal
    mainMenuFromGameOverButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    mainMenuFromGameOverButton.setPosition(centerX, windowHeight * 0.65f);
    mainMenuFromGameOverButton.setFillColor(sf::Color(255, 100, 100));
    
    mainMenuFromGameOverText.setFont(font);
    mainMenuFromGameOverText.setString("Menu Principal");
    mainMenuFromGameOverText.setCharacterSize(windowHeight * 0.04f);
    sf::FloatRect menuBounds = mainMenuFromGameOverText.getLocalBounds();
    mainMenuFromGameOverText.setPosition(
        centerX + (buttonWidth - menuBounds.width) / 2,
        windowHeight * 0.65f + (buttonHeight - menuBounds.height) / 2
    );
}
