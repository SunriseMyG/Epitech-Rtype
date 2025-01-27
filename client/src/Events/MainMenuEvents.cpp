/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Events
*/
#include "../../include/Menu/Menu.hpp"
#include "../../include/ECS/Entity.hpp"
#include "../../include/EntityFactory.hpp"
#include <iostream>
#include <algorithm>
#include <thread>



void Menu::handleReceiveMessageMainMenu() {
    while (networkManager.hasLoginMessages()) {
        std::string message = networkManager.getNextLoginMessage();
        if (message.find("$121") == 0) {
            if (message.find("KO") != std::string::npos) {
                size_t pos = message.find(": ");
                if (pos != std::string::npos) {
                    loginErrorMessage = message.substr(pos + 2);
                    showLoginError = true;
                }
            } else if (message.find("OK") != std::string::npos) {
                networkManager.requestRooms();
                networkManager.requestLeaderboard();
                setState(MenuState::ROOM_SELECT);
                showLoginError = false;
            }
        }
        if (message.find("$120") == 0) {
            if (message.find("KO") != std::string::npos) {
                size_t pos = message.find(": ");
                if (pos != std::string::npos) {
                    loginErrorMessage = message.substr(pos + 2);
                    showLoginError = true;
                }
            } else if (message.find("OK") != std::string::npos) {
                networkManager.loginfunc(loginString, passwordLoginString);
            }
            break;
        }

    }
}


void Menu::handleTextInput(std::string& text, sf::Uint32 unicode) {
    showLoginError = false; // Réinitialiser l'erreur quand l'utilisateur tape
    
    if (unicode == '\b' && !text.empty()) {
        text.pop_back();
    } else if (unicode < 128 && text.length() < 20) {
        text += static_cast<char>(unicode);
    }

    if (isLoginFocused) {
        loginText.setString(text);
    } else if (isPasswordLoginFocused) {
        std::string displayString(text.length(), '*');
        passwordLoginText.setString(displayString);
    }
}

void Menu::handleMainMenuEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);


    handleReceiveMessageMainMenu();


    if (event.type == sf::Event::MouseMoved) {
        if (!isHighContrastMode) {
            sf::Color defaultColor = isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(70, 70, 70);
            sf::Color hoverColor = isDaltonianMode ? sf::Color(0, 190, 255) : sf::Color(90, 90, 90);
            sf::Color joinDefaultColor = isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(90, 90, 90);
            sf::Color joinHoverColor = isDaltonianMode ? sf::Color(0, 190, 255) : sf::Color(110, 110, 110);
            sf::Color backgroundButton = isDaltonianMode ? sf::Color(50, 50, 150) : sf::Color(70, 70, 70);
            
            if (refreshButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                refreshButton.setFillColor(hoverColor);
            } else {
                refreshButton.setFillColor(defaultColor);
            }
            
            if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                backButton.setFillColor(hoverColor);
            } else {
                backButton.setFillColor(defaultColor);
            }
        
            for (auto& button : roomButtons) {
                if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    button.setFillColor(hoverColor);
                } else {
                    button.setFillColor(backgroundButton);
                }
            }
            
            if (createRoomButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                createRoomButton.setFillColor(hoverColor);
            } else {
                createRoomButton.setFillColor(defaultColor);
            }
            for (auto& button : joinButtons) {
                if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    button.setFillColor(joinHoverColor);
                } else {
                    button.setFillColor(joinDefaultColor);
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (accessibilityButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isHighContrastMode = !isHighContrastMode;
            
            // Forcer la mise à jour des styles
            updateAccessibilityStyles();
            if (isDaltonianMode) {
                applyDaltonianColors();
            }
            
            // Mettre à jour les éléments du menu
            updateRoomElements();
        } else if (loginInput.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isLoginFocused = true;
            isPasswordLoginFocused = false;
            return;
        } else if (passwordLoginInput.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isPasswordLoginFocused = true;
            isLoginFocused = false;
            return;
        } else if (loginButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.loginfunc(loginString, passwordLoginString);
        } else if (registerButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.registerfunc(loginString, passwordLoginString);
        } else if (quitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            window.close();
            networkManager.disconnect();
            return;
        } else {
            isLoginFocused = false;
            isPasswordLoginFocused = false;
        }
    }


    if (event.type == sf::Event::TextEntered) {
        if (isLoginFocused) {
            handleTextInput(loginString, event.text.unicode);
            loginText.setString(loginString);
        }
        else if (isPasswordLoginFocused) {
            handleTextInput(passwordLoginString, event.text.unicode);
            std::string displayString(passwordLoginString.length(), '*');
            passwordLoginText.setString(displayString);
        }
    }
}
