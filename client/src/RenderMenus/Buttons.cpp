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

void Menu::createButtons() {
    configureMainButtons();
    configureButtonText();
    configureAdditionalButtons();
    configureInputFields();
    configureAccessibilitySettings();
}

void Menu::configureMainButtons() {
}

void Menu::configureButtonText() {
}

void Menu::configureAdditionalButtons() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float textSize = windowHeight * 0.04f;
    
    // Configuration du bouton retour
    float backButtonWidth = windowWidth * 0.12f;
    float backButtonHeight = windowHeight * 0.05f;
    
    backButton.setSize(sf::Vector2f(backButtonWidth, backButtonHeight));
    backButton.setPosition(windowWidth * 0.05f, windowHeight * 0.9f);
    backButton.setFillColor(sf::Color(100, 100, 100));
    
    backText.setFont(font);
    backText.setString("Retour");
    backText.setCharacterSize(textSize * 0.8f);
    
    sf::FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        windowWidth * 0.05f + (backButtonWidth - backBounds.width) / 2,
        windowHeight * 0.9f + (backButtonHeight - backBounds.height) / 2 - backBounds.top
    );
    
    // Configuration du bouton refresh
    float refreshButtonWidth = windowWidth * 0.12f;
    float refreshButtonHeight = windowHeight * 0.05f;
    
    refreshButton.setSize(sf::Vector2f(refreshButtonWidth, refreshButtonHeight));
    refreshButton.setPosition(windowWidth * 0.83f, windowHeight * 0.9f);
    refreshButton.setFillColor(sf::Color(70, 70, 70));
    refreshButton.setOutlineThickness(2);
    refreshButton.setOutlineColor(sf::Color(100, 100, 100));
    
    refreshText.setFont(font);
    refreshText.setString("Rafraichir");
    refreshText.setCharacterSize(textSize * 0.8f);
    
    sf::FloatRect refreshBounds = refreshText.getLocalBounds();
    refreshText.setPosition(
        windowWidth * 0.83f + (refreshButtonWidth - refreshBounds.width) / 2,
        windowHeight * 0.9f + (refreshButtonHeight - refreshBounds.height) / 2 - refreshBounds.top
    );
}

void Menu::configureInputFields() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float inputWidth = windowWidth * 0.25f;
    float inputHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - inputWidth) / 2;
    float leftbottomX = windowWidth * 0.05f;

    // Positions Y pour les champs et boutons
    float usernameY = windowHeight * 0.3f;
    float passwordY = windowHeight * 0.45f;
    float loginY = windowHeight * 0.65f;
    float registerY = windowHeight * 0.75f;
    float quitY = windowHeight * 0.85f; // Position pour le bouton "Quitter"

    // Configuration des champs de saisie
    loginInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    loginInput.setPosition(centerX, usernameY);
    loginInput.setFillColor(sf::Color(50, 50, 50));
    loginInput.setOutlineThickness(2);
    loginInput.setOutlineColor(sf::Color(150, 150, 150));

    passwordLoginInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    passwordLoginInput.setPosition(centerX, passwordY);
    passwordLoginInput.setFillColor(sf::Color(50, 50, 50));
    passwordLoginInput.setOutlineThickness(2);
    passwordLoginInput.setOutlineColor(sf::Color(150, 150, 150));

    // Configuration des boutons
    loginButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    loginButton.setPosition(centerX, loginY);
    loginButton.setFillColor(sf::Color(100, 100, 100));

    registerButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    registerButton.setPosition(centerX, registerY);
    registerButton.setFillColor(sf::Color(100, 100, 100));
    registerButton.setOutlineThickness(2);
    registerButton.setOutlineColor(sf::Color(150, 150, 150));

    // Configuration du bouton "Quitter"
    quitButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    quitButton.setPosition(leftbottomX, quitY);
    quitButton.setFillColor(sf::Color(100, 100, 100));
    quitButton.setOutlineThickness(2);
    quitButton.setOutlineColor(sf::Color(150, 150, 150));

    // Configuration des textes
    float textSize = windowHeight * 0.03f;
    loginText.setFont(font);
    loginText.setCharacterSize(textSize);
    loginText.setPosition(
        loginInput.getPosition().x + 10,
        loginInput.getPosition().y + (loginInput.getSize().y - loginText.getCharacterSize()) / 2
    );

    passwordLoginText.setFont(font);
    passwordLoginText.setCharacterSize(textSize);
    passwordLoginText.setPosition(
        passwordLoginInput.getPosition().x + 10,
        passwordLoginInput.getPosition().y + (passwordLoginInput.getSize().y - passwordLoginText.getCharacterSize()) / 2
    );

    loginButtonText.setFont(font);
    loginButtonText.setString("Connexion");
    loginButtonText.setCharacterSize(textSize);
    centerTextInButton(loginButtonText, loginButton);

    registerButtonText.setFont(font);
    registerButtonText.setString("S'inscrire");
    registerButtonText.setCharacterSize(textSize);
    centerTextInButton(registerButtonText, registerButton);

    // Texte du bouton "Quitter"
    quitText.setFont(font);
    quitText.setString("Quitter");
    quitText.setCharacterSize(textSize);
    centerTextInButton(quitText, quitButton);

    // Configuration des étiquettes
    loginLabel.setFont(font);
    loginLabel.setString("Nom d'utilisateur:");
    loginLabel.setCharacterSize(textSize);
    loginLabel.setPosition(centerX, usernameY - 30);

    passwordLoginLabel.setFont(font);
    passwordLoginLabel.setString("Mot de passe:");
    passwordLoginLabel.setCharacterSize(textSize);
    passwordLoginLabel.setPosition(centerX, passwordY - 30);
}

void Menu::configureAccessibilitySettings() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    float accessibilityButtonWidth = windowWidth * 0.12f;
    float accessibilityButtonHeight = windowHeight * 0.05f;
    
    accessibilityButton.setSize(sf::Vector2f(accessibilityButtonWidth, accessibilityButtonHeight));
    accessibilityButton.setPosition(windowWidth * 0.05f, windowHeight * 0.05f);
    accessibilityButton.setFillColor(sf::Color(100, 100, 100));
    
    accessibilityText.setFont(font);
    accessibilityText.setString("Accessibilite");
    accessibilityText.setCharacterSize(windowHeight * 0.04f * 0.8f);
    
    sf::FloatRect accessBounds = accessibilityText.getLocalBounds();
    accessibilityText.setPosition(
        windowWidth * 0.05f + (accessibilityButtonWidth - accessBounds.width) / 2,
        windowHeight * 0.05f + (accessibilityButtonHeight - accessBounds.height) / 2
    );
    
    isHighContrastMode = false;
    textScaleFactor = 1.0f;
}
