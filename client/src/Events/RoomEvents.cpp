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
#include <chrono>




void Menu::handleRoomCreateEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseButtonPressed) {
        // Gestion du focus des inputs
        isNameFocused = nameInput.getGlobalBounds().contains(mousePos.x, mousePos.y);
        isPasswordFocused = passwordInput.getGlobalBounds().contains(mousePos.x, mousePos.y);
        isMaxPlayersFocused = maxPlayersInput.getGlobalBounds().contains(mousePos.x, mousePos.y);
        
        // Mise à jour des couleurs des bordures
        nameInput.setOutlineColor(isNameFocused ? sf::Color(200, 200, 200) : sf::Color(150, 150, 150));
        passwordInput.setOutlineColor(isPasswordFocused ? sf::Color(200, 200, 200) : sf::Color(150, 150, 150));
        maxPlayersInput.setOutlineColor(isMaxPlayersFocused ? sf::Color(200, 200, 200) : sf::Color(150, 150, 150));
        
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.requestLeaderboard();
            networkManager.requestRooms();
            setState(MenuState::ROOM_SELECT);
            return;
        } else if (submitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (!nameString.empty() && !maxPlayersString.empty()) {
                // Activer le loading sur le bouton submit
                isLoading = true;
                loadingClock.restart();
                submitButton.setFillColor(sf::Color(100, 100, 100));
                
                // Créer la room et attendre
                std::string command = "102 " + nameString + " " + passwordString + " " + maxPlayersString;
                networkManager.sendTCP(command);
                networkManager.requestRooms();
                setState(MenuState::ROOM_SELECT);
                return;

            }
        } else if (nameInput.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isNameFocused = true;
            isPasswordFocused = false;
            isMaxPlayersFocused = false;
        } else if (passwordInput.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isNameFocused = false;
            isPasswordFocused = true;
            isMaxPlayersFocused = false;
        } else if (maxPlayersInput.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isNameFocused = false;
            isPasswordFocused = false;
            isMaxPlayersFocused = true;
        }
    }
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b') {
            if (isNameFocused && !nameString.empty())
                nameString.pop_back();
            else if (isPasswordFocused && !passwordString.empty())
                passwordString.pop_back();
            else if (isMaxPlayersFocused && !maxPlayersString.empty())
                maxPlayersString.pop_back();
        }
        else if (event.text.unicode < 128) {
            if (isNameFocused)
                nameString += static_cast<char>(event.text.unicode);
            else if (isPasswordFocused)
                passwordString += static_cast<char>(event.text.unicode);
            else if (isMaxPlayersFocused && std::isdigit(event.text.unicode))
                maxPlayersString += static_cast<char>(event.text.unicode);
        }
        
        nameText.setString(nameString);
        passwordText.setString(std::string(passwordString.length(), '*'));
        maxPlayersText.setString(maxPlayersString);
    }
}

void Menu::handlePasswordEvents(const sf::Event& event) {
    if (!isPasswordPromptVisible) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    // Gestion de la saisie du mot de passe
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b' && !passwordPromptString.empty()) {
            passwordPromptString.pop_back();
        } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
            passwordPromptString += static_cast<char>(event.text.unicode);
        }
        passwordPromptText.setString(std::string(passwordPromptString.length(), '*'));
    }

    // Gestion des clics sur les boutons
    if (event.type == sf::Event::MouseButtonPressed) {
        if (passwordPromptBack.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isPasswordPromptVisible = false;
            passwordPromptString.clear();
            passwordPromptText.setString("");
        }

        if (passwordPromptSubmit.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.joinRoom(currentRoomId, passwordPromptString);
            while (true) {
                std::string message = networkManager.getNextRoomMessage();
                if (message.find("$103 OK") != std::string::npos) {
                    isPasswordPromptVisible = false;
                    networkManager.requestPlayersInRoom();
                    currentState = MenuState::ROOM_LOBBY;
                    break;
                } else if (message.find("$103 KO") != std::string::npos) {
                    passwordPromptString.clear();
                    passwordPromptText.setString("");
                    break;
                }
            }
        }
    }
}


void Menu::handleRoomLobbyEvents(const sf::Event& event) {
    handleChatEvents(event);
    //networkManager.requestPlayersInRoom();
    while (networkManager.hasMessages()) {
        std::string message = networkManager.getNextMessage(false);
        std::cout << "Message reçu dans le lobby: " << message << std::endl;
        
        if (!message.empty()) {
            if (message[0] == '$') {
                std::string protocol_id = message.substr(1, 3);
                std::string content = message.substr(5);
                
                if (protocol_id == "105") {
                    setState(MenuState::GAME);
                    networkManager.sendUDP("115");
                    return;
                } else if (protocol_id == "123" || protocol_id == "124" || protocol_id == "122") {
                    bool isPrivate = (protocol_id == "122");
                    addChatMessage(content, isPrivate);  // Ajout du paramètre isPrivate
                } else if (protocol_id == "119" || protocol_id == "112") {
                    std::cout << "Message reçu dans le lobby: " << content << std::endl;
                    updateLobbyPlayers(content);
                }
            }
        }
    }


    // Gestion des événements de l'interface
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        sf::Color defaultBackColor = isDaltonianMode ? sf::Color(0, 170, 230) : 
            (isHighContrastMode ? sf::Color::White : sf::Color(100, 100, 100));
        sf::Color hoverBackColor = isDaltonianMode ? sf::Color(0, 190, 255) : 
            (isHighContrastMode ? sf::Color(230, 230, 230) : sf::Color(120, 120, 120));
        
        sf::Color defaultStartColor = isDaltonianMode ? sf::Color(0, 170, 230) : 
            (isHighContrastMode ? sf::Color::White : sf::Color(0, 150, 0));
        sf::Color hoverStartColor = isDaltonianMode ? sf::Color(0, 190, 255) : 
            (isHighContrastMode ? sf::Color(230, 230, 230) : sf::Color(0, 180, 0));
        
        // Effet hover pour le bouton retour
        if (lobbybackButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            lobbybackButton.setFillColor(hoverBackColor);
        } else {
            lobbybackButton.setFillColor(defaultBackColor);
        }
        
        // Effet hover pour le bouton start
        if (lobbystartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            lobbystartButton.setFillColor(hoverStartColor);
        } else {
            lobbystartButton.setFillColor(defaultStartColor);
        }
        
        // Effet hover pour le bouton d'accessibilité
        if (accessibilityButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            accessibilityButton.setFillColor(sf::Color(120, 120, 120));
        } else {
            accessibilityButton.setFillColor(sf::Color(100, 100, 100));
        }

        // Couleurs par défaut
        sf::Color defaultColor = sf::Color(120, 120, 120);
        sf::Color hoverColor = sf::Color(140, 140, 140);
        
        // Effet de survol pour les boutons de difficulté
        if (!lobbyPlayers.empty() && std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0]) {
            if (difficultyEasyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != &difficultyEasyButton) {
                    difficultyEasyButton.setFillColor(sf::Color(110, 110, 110));
                }
            } else if (activeDifficultyButton != &difficultyEasyButton) {
                difficultyEasyButton.setFillColor(sf::Color(100, 100, 100));
            }
            
            if (difficultyMediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != &difficultyMediumButton) {
                    difficultyMediumButton.setFillColor(sf::Color(110, 110, 110));
                }
            } else if (activeDifficultyButton != &difficultyMediumButton) {
                difficultyMediumButton.setFillColor(sf::Color(100, 100, 100));
            }
            
            if (difficultyHardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != &difficultyHardButton) {
                    difficultyHardButton.setFillColor(sf::Color(110, 110, 110));
                }
            } else if (activeDifficultyButton != &difficultyHardButton) {
                difficultyHardButton.setFillColor(sf::Color(100, 100, 100));
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        if (accessibilityButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isHighContrastMode = !isHighContrastMode;
            
            // Forcer la mise à jour des styles
            updateAccessibilityStyles();
            if (isDaltonianMode) {
                applyDaltonianColors();
            } else {
                // Réinitialiser les couleurs des rooms si ni daltonien ni contraste élevé
                sf::Color defaultColor = sf::Color(70, 70, 70);
                for (auto& button : roomButtons) {
                    button.setFillColor(defaultColor);
                }
                for (auto& button : joinButtons) {
                    button.setFillColor(defaultColor);
                }
            }
            
            // Mettre à jour les éléments de la room
            updateRoomElements();
        } else if (lobbybackButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.leaveRoom(currentRoomId);
            networkManager.requestRooms();
            setState(MenuState::ROOM_SELECT);
            return;
        } 
        else if (!lobbyPlayers.empty() && 
                 std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0] &&
                 lobbystartButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            // Nettoyer toutes les entités existantes avant de démarrer une nouvelle partie
            if (world) {
                auto& entities = world->getEntities();
                for (auto& entity : entities) {
                    if (entity) {
                        entity->markForDelete();
                    }
                }
            }
            networkManager.startGame(currentRoomId);
            setState(MenuState::GAME);
            return;
        }
        
        // Gestion des boutons de difficulté (uniquement pour le leader)
        if (!lobbyPlayers.empty() && std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0]) {
            if (difficultyEasyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != nullptr) {
                    activeDifficultyButton->setFillColor(sf::Color(100, 100, 100));
                }
                activeDifficultyButton = &difficultyEasyButton;
                activeDifficultyButton->setFillColor(sf::Color(120, 120, 120));
                currentDifficulty = "EASY";
                networkManager.sendTCP("118 EASY");
            } 
            else if (difficultyMediumButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != nullptr) {
                    activeDifficultyButton->setFillColor(sf::Color(100, 100, 100));
                }
                activeDifficultyButton = &difficultyMediumButton;
                activeDifficultyButton->setFillColor(sf::Color(120, 120, 120));
                currentDifficulty = "MEDIUM";
                networkManager.sendTCP("118 MEDIUM");
            } 
            else if (difficultyHardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                if (activeDifficultyButton != nullptr) {
                    activeDifficultyButton->setFillColor(sf::Color(100, 100, 100));
                }
                activeDifficultyButton = &difficultyHardButton;
                activeDifficultyButton->setFillColor(sf::Color(120, 120, 120));
                currentDifficulty = "HARD";
                networkManager.sendTCP("118 HARD");
            }
        }
    }
}


void Menu::handleRoomSelectEvents(const sf::Event& event) {
    handleChatEvents(event);
    while (networkManager.hasMessages()) {
        std::string message = networkManager.getNextMessage();
        if (!message.empty()) {
            if (message[0] == '$') {
                std::string protocol_id = message.substr(1, 3);
                std::string content = message.substr(5);
                if (protocol_id == "123" || protocol_id == "124" || protocol_id == "122") {
                    bool isPrivate = (protocol_id == "122");
                    addChatMessage(content, isPrivate);  // Ajout du paramètre isPrivate
                } 
            }
        }
    }
    while (networkManager.hasRoomListMessages()) {
        std::string message = networkManager.getNextRoomListMessage();
        if (!message.empty()) {
            std::string protocol_id = message.substr(1, 3);
            std::string content = message.substr(5);
            if (protocol_id == "104") {
                updateRooms(content);
                break;
            }
        }
    }



    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseMoved) {
        if (!isHighContrastMode) {
            sf::Color defaultColor = isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(70, 70, 70);
            sf::Color hoverColor = isDaltonianMode ? sf::Color(0, 190, 255) : sf::Color(90, 90, 90);
            sf::Color roomDefaultColor = isDaltonianMode ? sf::Color(50, 50, 150) : sf::Color(70, 70, 70);
            sf::Color roomHoverColor = isDaltonianMode ? sf::Color(60, 60, 160) : sf::Color(90, 90, 90);
            
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
            
            // Effet de survol pour les boutons de room
            for (auto& button : roomButtons) {
                if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    button.setFillColor(roomHoverColor);
                } else {
                    button.setFillColor(roomDefaultColor);
                }
            }
            
            if (createRoomButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                createRoomButton.setFillColor(hoverColor);
            } else {
                createRoomButton.setFillColor(defaultColor);
            }
            
            // Effet de survol pour les boutons de jointure
            for (auto& button : joinButtons) {
                if (button.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    button.setFillColor(hoverColor);
                } else {
                    button.setFillColor(defaultColor);
                }
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed) {
        if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.leaveRoom(currentRoomId);
            networkManager.requestPlayersInRoom();
            setState(MenuState::MAIN_MENU);
        } else if (refreshButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (!isLoading && !isRefreshing) {  // Vérification supplémentaire
                isLoading = true;
                loadingClock.restart();
                networkManager.requestRooms();
                networkManager.requestLeaderboard();
            }
        } else if (searchBar.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isSearchBarFocused = true;
        } else if (createRoomButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            setState(MenuState::ROOM_CREATE);
        } else if (matchmakingButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            networkManager.sendTCP("127");
            while (true) {
                std::string message = networkManager.getNextRoomMessage();
                if (message.find("$127 OK") != std::string::npos) {
                    std::cout << "OK" << std::endl;
                    networkManager.requestPlayersInRoom();
                    currentState = MenuState::ROOM_LOBBY;
                    break;
                } else if (message.find("$127 KO") != std::string::npos) {
                    std::cerr << "Erreur: Impossible de rejoindre la salle." << std::endl;
                    break;
                }
            }
            return;
        }
        // Gestion des clics sur les boutons de jointure
        for (size_t i = 0; i < joinButtons.size(); ++i) {
            if (joinButtons[i].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                std::string roomName = roomTexts[i * 2].getString();
                size_t protectedPos = roomName.find(" [Protected]");
                if (protectedPos != std::string::npos) {
                    roomName = roomName.substr(0, protectedPos);
                    for (const auto& room : rooms) {
                        if (room.name == roomName) {
                            currentRoomId = room.id;
                            isPasswordPromptVisible = true;
                            passwordPromptString = "";
                            passwordPromptText.setString("");
                            break;
                        }
                    }
                } else {
                    networkManager.joinRoom(rooms[i].id, "");
                    std::cout << "joinRoom" << std::endl;
                    while (true) {
                        std::string message = networkManager.getNextRoomMessage();
                        if (message.find("$103 OK") != std::string::npos) {
                            std::cout << "OK" << std::endl;
                            isPasswordPromptVisible = false;
                            networkManager.requestPlayersInRoom();
                            currentState = MenuState::ROOM_LOBBY;
                            break;
                        } else if (message.find("$103 KO") != std::string::npos) {
                            std::cerr << "Erreur: Impossible de rejoindre la salle." << std::endl;
                            break;
                        }
                    }
                    return;
                }
            }
        }
    }

}

void Menu::renderPasswordPrompt() {
    if (!isPasswordPromptVisible) return;

    // Overlay semi-transparent
    sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    overlay.setFillColor(sf::Color(0, 0, 0, 128));
    window.draw(overlay);

    // Couleurs adaptées au mode d'accessibilité
    sf::Color bgColor = isHighContrastMode ? sf::Color::Black : sf::Color(50, 50, 50);
    sf::Color textColor = isHighContrastMode ? sf::Color::White : sf::Color::White;
    sf::Color inputBgColor = isHighContrastMode ? sf::Color::White : sf::Color(30, 30, 30);
    sf::Color buttonColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);
    sf::Color outlineColor = isHighContrastMode ? sf::Color::White : sf::Color(100, 100, 100);

    // Boîte de dialogue
    float promptWidth = window.getSize().x * 0.4f;
    float promptHeight = window.getSize().y * 0.3f;
    float centerX = (window.getSize().x - promptWidth) / 2;
    float centerY = (window.getSize().y - promptHeight) / 2;

    // Fond
    sf::RectangleShape promptBackground(sf::Vector2f(promptWidth, promptHeight));
    promptBackground.setPosition(centerX, centerY);
    promptBackground.setFillColor(bgColor);
    promptBackground.setOutlineThickness(2.0f);
    promptBackground.setOutlineColor(outlineColor);
    window.draw(promptBackground);

    // Label
    passwordPromptLabel.setString("Mot de passe :");
    passwordPromptLabel.setFillColor(textColor);
    passwordPromptLabel.setPosition(centerX + 20, centerY + 30);
    window.draw(passwordPromptLabel);

    // Champ de saisie
    float inputWidth = promptWidth * 0.8f;
    float inputHeight = promptHeight * 0.2f;
    passwordPromptInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    passwordPromptInput.setPosition(centerX + (promptWidth - inputWidth) / 2, centerY + promptHeight * 0.3f);
    passwordPromptInput.setFillColor(inputBgColor);
    passwordPromptInput.setOutlineThickness(2.0f);
    passwordPromptInput.setOutlineColor(outlineColor);
    window.draw(passwordPromptInput);

    // Texte masqué
    passwordPromptText.setFillColor(isHighContrastMode ? sf::Color::Black : sf::Color::White);
    passwordPromptText.setString(std::string(passwordPromptString.length(), '*'));
    passwordPromptText.setPosition(
        passwordPromptInput.getPosition().x + 10,
        passwordPromptInput.getPosition().y + (inputHeight - passwordPromptText.getCharacterSize()) / 2
    );
    window.draw(passwordPromptText);

    // Boutons
    float buttonWidth = promptWidth * 0.35f;
    float buttonHeight = promptHeight * 0.2f;
    float buttonY = centerY + promptHeight - buttonHeight - 20;

    // Bouton Retour
    passwordPromptBack.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    passwordPromptBack.setPosition(centerX + 20, buttonY);
    passwordPromptBack.setFillColor(buttonColor);
    passwordPromptBack.setOutlineThickness(2.0f);
    passwordPromptBack.setOutlineColor(outlineColor);
    window.draw(passwordPromptBack);

    passwordPromptBackText.setFillColor(isHighContrastMode ? sf::Color::Black : sf::Color::White);
    passwordPromptBackText.setString("Retour");
    sf::FloatRect backBounds = passwordPromptBackText.getLocalBounds();
    passwordPromptBackText.setPosition(
        passwordPromptBack.getPosition().x + (buttonWidth - backBounds.width) / 2,
        buttonY + (buttonHeight - backBounds.height) / 2
    );
    window.draw(passwordPromptBackText);

    // Bouton Valider
    passwordPromptSubmit.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    passwordPromptSubmit.setPosition(centerX + promptWidth - buttonWidth - 20, buttonY);
    passwordPromptSubmit.setFillColor(buttonColor);
    passwordPromptSubmit.setOutlineThickness(2.0f);
    passwordPromptSubmit.setOutlineColor(outlineColor);
    window.draw(passwordPromptSubmit);

    passwordPromptSubmitText.setFillColor(isHighContrastMode ? sf::Color::Black : sf::Color::White);
    passwordPromptSubmitText.setString("Valider");
    sf::FloatRect submitBounds = passwordPromptSubmitText.getLocalBounds();
    passwordPromptSubmitText.setPosition(
        passwordPromptSubmit.getPosition().x + (buttonWidth - submitBounds.width) / 2,
        buttonY + (buttonHeight - submitBounds.height) / 2
    );
    window.draw(passwordPromptSubmitText);
}
