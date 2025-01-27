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


void Menu::renderMainMenu() {
    window.draw(loginInput);
    window.draw(passwordLoginInput);
    window.draw(loginButton);
    window.draw(registerButton);
    window.draw(loginLabel);
    window.draw(passwordLoginLabel);
    window.draw(loginButtonText);
    window.draw(registerButtonText);
    window.draw(loginText);
    window.draw(passwordLoginText);
    window.draw(quitButton);
    window.draw(quitText);

    if (isLoginFocused || isPasswordLoginFocused) {
        sf::RectangleShape cursor;
        cursor.setSize(sf::Vector2f(2, loginInput.getSize().y - 10));
        cursor.setFillColor(sf::Color::White);

        if (isLoginFocused) {
            cursor.setPosition(
                loginText.getPosition().x + loginText.getLocalBounds().width + 2,
                loginInput.getPosition().y + 5
            );
        } else {
            cursor.setPosition(
                passwordLoginText.getPosition().x + passwordLoginText.getLocalBounds().width + 2,
                passwordLoginInput.getPosition().y + 5
            );
        }
        window.draw(cursor);
    }
    if (showLoginError) {
        sf::Text errorText;
        errorText.setFont(font);
        errorText.setString(loginErrorMessage);
        errorText.setCharacterSize(20);
        errorText.setFillColor(sf::Color::Red);
        
        sf::Vector2f passwordPos = passwordLoginInput.getPosition();
        errorText.setPosition(
            passwordPos.x,
            passwordPos.y + passwordLoginInput.getSize().y + 10
        );
        window.draw(errorText);
    }
}

void Menu::resizeMainMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    // Configuration des champs de saisie
    float inputWidth = windowWidth * 0.25f;
    float inputHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - inputWidth) / 2;

    // Positions verticales
    float usernameY = windowHeight * 0.3f;
    float passwordY = windowHeight * 0.45f;
    float loginY = windowHeight * 0.65f;
    float registerY = windowHeight * 0.75f;
    float quitY = windowHeight * 0.85f;

    // Redimensionnement des champs de saisie
    loginInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    loginInput.setPosition(centerX, usernameY);

    passwordLoginInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    passwordLoginInput.setPosition(centerX, passwordY);

    // Redimensionnement des boutons
    loginButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    loginButton.setPosition(centerX, loginY);

    registerButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    registerButton.setPosition(centerX, registerY);

    quitButton.setSize(sf::Vector2f(inputWidth, inputHeight));
    quitButton.setPosition(centerX, quitY);

    // Mise à jour de la taille du texte
    float textSize = windowHeight * 0.03f;

    // Labels
    loginLabel.setCharacterSize(textSize);
    loginLabel.setPosition(centerX, usernameY - 30);

    passwordLoginLabel.setCharacterSize(textSize);
    passwordLoginLabel.setPosition(centerX, passwordY - 30);

    // Textes des boutons
    loginButtonText.setCharacterSize(textSize);
    registerButtonText.setCharacterSize(textSize);
    quitText.setCharacterSize(textSize);

    // Centrage des textes dans les boutons
    centerTextInButton(loginButtonText, loginButton);
    centerTextInButton(registerButtonText, registerButton);
    centerTextInButton(quitText, quitButton);

    // Mise à jour des textes de saisie
    loginText.setCharacterSize(textSize);
    loginText.setPosition(
        loginInput.getPosition().x + 10,
        loginInput.getPosition().y + (loginInput.getSize().y - loginText.getCharacterSize()) / 2
    );

    passwordLoginText.setCharacterSize(textSize);
    passwordLoginText.setPosition(
        passwordLoginInput.getPosition().x + 10,
        passwordLoginInput.getPosition().y + (passwordLoginInput.getSize().y - passwordLoginText.getCharacterSize()) / 2
    );

    // Mise à jour du bouton d'accessibilité
    float accessibilityButtonWidth = windowWidth * 0.12f;
    float accessibilityButtonHeight = windowHeight * 0.05f;
    
    accessibilityButton.setSize(sf::Vector2f(accessibilityButtonWidth, accessibilityButtonHeight));
    accessibilityButton.setPosition(windowWidth * 0.05f, windowHeight * 0.05f);
    
    accessibilityText.setCharacterSize(windowHeight * 0.03f);
    centerTextInButton(accessibilityText, accessibilityButton);
}

