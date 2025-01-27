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

void Menu::updateLobbyPlayers(const std::string& playerData) {
    try {
        lobbyPlayers.clear();
        std::istringstream iss(playerData);
        std::string id;
        while (iss >> id) {
            lobbyPlayers.push_back(id);
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du parsing des joueurs: " << e.what() << std::endl;
    }
}

void Menu::updateRoomListPositions() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float ySpacing = windowHeight * 0.1f;
    float startY = windowHeight * 0.2f;
    
    for (size_t i = 0; i < roomButtons.size(); ++i) {
        float buttonWidth = windowWidth * 0.5f;
        float buttonHeight = windowHeight * 0.08f;
        float centerX = (windowWidth - buttonWidth) / 2;
        
        roomButtons[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
        roomButtons[i].setPosition((windowWidth - buttonWidth) / 2, startY + (i * ySpacing));
        
        if (i * 2 < roomTexts.size()) {
            roomTexts[i * 2].setCharacterSize(windowHeight * 0.03f);
            roomTexts[i * 2].setPosition(
                centerX + 10,
                startY + (i * ySpacing) + buttonHeight * 0.25f
            );
        }
    }
}

void Menu::initializeRooms() {
    try {
        if (!networkManager.isConnected()) {
            networkManager.connect();
        }
        networkManager.startReceiving();
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de l'initialisation des rooms: " << e.what() << std::endl;
    }
    
    roomButtons.clear();
    roomTexts.clear();
}

void Menu::updateRooms(const std::string& roomData) {
    try {
        if (roomData.empty()) {
            std::cerr << "Données de room vides" << std::endl;
            return;
        }

        // Vérifier si les données sont identiques
        static std::string lastRoomData;
        if (lastRoomData == roomData) {
            isLoading = false;
            isRefreshing = false;
            std::cout << "Données identiques, retour" << std::endl;
            return;
        }
        lastRoomData = roomData;

        std::cout << "Update rooms : " << roomData << std::endl;
        JsonParser parser(roomData);
        auto parsedRooms = parser.parseRooms();

        // Nettoyer les conteneurs de manière sécurisée
        rooms.clear();
        roomButtons.clear();
        roomTexts.clear();
        joinButtons.clear();
        joinButtonTexts.clear();

        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;
        float yPos = 100.0f;
        const float MAX_ROOMS = 10; // Limite le nombre de rooms affichées
        size_t roomCount = 0;

        // Conversion et affichage des rooms
        for (const auto& parsedRoom : parsedRooms) {
            if (roomCount >= MAX_ROOMS) break;
            
            // Vérifier l'état de la room
            if (parsedRoom.state == "GAME") continue;

            // Créer et ajouter la room à la liste
            Room room;
            room.id = parsedRoom.id;
            room.name = parsedRoom.name.empty() ? "Sans nom" : parsedRoom.name;
            room.currentPlayers = parsedRoom.currentPlayers;
            room.maxPlayers = parsedRoom.maxPlayers;
            room.password = parsedRoom.password;
            room.state = parsedRoom.state;
            rooms.push_back(room);

            try {
                // Créer le bouton de la room avec vérification des dimensions
                sf::RectangleShape button(sf::Vector2f(std::min(400.0f, windowWidth * 0.4f), 50.0f));
                button.setPosition(windowWidth / 2 - button.getSize().x / 2, yPos);
                button.setFillColor(sf::Color(120, 120, 120));
                roomButtons.push_back(button);

                // Créer le bouton Rejoindre
                sf::RectangleShape joinButton(sf::Vector2f(100.0f, 40.0f));
                joinButton.setPosition(
                    std::min(windowWidth / 2 + 160.0f, windowWidth - 120.0f),
                    yPos + 5.0f
                );
                joinButton.setFillColor(sf::Color(140, 140, 140));
                joinButtons.push_back(joinButton);

                // Texte du bouton Rejoindre
                sf::Text joinText;
                joinText.setFont(robotoFont);
                joinText.setString("Rejoindre");
                joinText.setCharacterSize(16);
                joinText.setFillColor(sf::Color::White);
                
                // Centrer le texte de manière sécurisée
                sf::FloatRect joinBounds = joinText.getLocalBounds();
                joinText.setPosition(
                    joinButton.getPosition().x + (joinButton.getSize().x - joinBounds.width) / 2,
                    joinButton.getPosition().y + (joinButton.getSize().y - joinBounds.height) / 2
                );
                joinButtonTexts.push_back(joinText);

                // Textes de la room
                sf::Text nameText;
                nameText.setFont(robotoFont);
                std::string displayName = room.name;
                if (!room.password.empty()) {
                    displayName += " [Protected]";
                }
                nameText.setString(displayName);
                nameText.setCharacterSize(20);
                nameText.setFillColor(sf::Color::White);
                nameText.setPosition(button.getPosition().x + 10, yPos + 10);
                roomTexts.push_back(nameText);

                // Texte des joueurs
                sf::Text playersText;
                playersText.setFont(robotoFont);
                playersText.setCharacterSize(20);

                int current = 0, max = 0;
                try {
                    current = std::stoi(room.currentPlayers);
                    max = std::stoi(room.maxPlayers);
                } catch (...) {
                    current = 0;
                    max = 0;
                }

                playersText.setString("(" + std::to_string(current) + "/" + std::to_string(max) + ")");
                playersText.setFillColor(current >= max ? sf::Color::Red : sf::Color::Green);
                
                // Positionner le texte à gauche du bouton Rejoindre
                float joinButtonX = std::min(windowWidth / 2 + 160.0f, windowWidth - 120.0f);
                playersText.setPosition(
                    joinButtonX - playersText.getGlobalBounds().width - 10,  // 10 pixels de marge
                    yPos + 15.0f  // Centré verticalement avec le bouton
                );
                roomTexts.push_back(playersText);

                yPos += 70.0f;
                roomCount++;
            } catch (const std::exception& e) {
                std::cerr << "Erreur lors de la création des éléments graphiques: " << e.what() << std::endl;
                continue;
            }
        }
        handleResize();
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du parsing des rooms: " << e.what() << std::endl;
    }
}


void Menu::renderRoomSelect() {
    float windowHeight = window.getSize().y;
    searchText.setFont(robotoFont);
    searchText.setCharacterSize(windowHeight * 0.03f);
    searchText.setFillColor(sf::Color::Black);

    window.draw(searchBar);
    window.draw(searchText);
    window.draw(createRoomButton);
    window.draw(createRoomText);

    for (size_t i = 0; i < roomButtons.size(); ++i) {
        std::string roomName = roomTexts[i * 2].getString();
        size_t protectedPos = roomName.find(" [Protected]");
        if (protectedPos != std::string::npos) {
            roomName = roomName.substr(0, protectedPos);
        }
        
        std::string lowerRoomName = roomName;
        std::string lowerSearchString = searchString;
        std::transform(lowerRoomName.begin(), lowerRoomName.end(), lowerRoomName.begin(), ::tolower);
        std::transform(lowerSearchString.begin(), lowerSearchString.end(), lowerSearchString.begin(), ::tolower);
        
        if (searchString.empty() || lowerRoomName.find(lowerSearchString) != std::string::npos) {
            window.draw(roomButtons[i]);
            window.draw(roomTexts[i * 2]);
            window.draw(roomTexts[i * 2 + 1]);
            window.draw(joinButtons[i]);
            window.draw(joinButtonTexts[i]);
        }
    }
    
    window.draw(backButton);
    window.draw(backText);
    window.draw(refreshButton);
    
    if (isLoading) {
        loadingSpinner.setPosition(
            refreshButton.getPosition().x + refreshButton.getSize().x / 2,
            refreshButton.getPosition().y + refreshButton.getSize().y / 2
        );
        window.draw(loadingSpinner);
    } else {
        window.draw(refreshText);
    }
    
    renderChat();
    updateLoadingSpinner();

    if (isPasswordPromptVisible) {
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);
        window.draw(passwordPromptBack);
        window.draw(passwordPromptBackText);
        window.draw(passwordPromptInput);
        window.draw(passwordPromptLabel);
        window.draw(passwordPromptText);
        window.draw(passwordPromptSubmit);
        window.draw(passwordPromptSubmitText);
    }
    if (isScoreboardVisible) {
        window.draw(scoreboardBackground);
        window.draw(scoreboardTitle);
        for (const auto& entry : scoreboardEntries) {
            window.draw(entry);
        }
    }

    // Dessiner le bouton de matchmaking

    window.draw(matchmakingButton);
    window.draw(matchmakingText);
}



void Menu::renderRoomCreate() {
    window.draw(nameInput);
    window.draw(nameLabel);
    window.draw(nameText);
    if (isNameFocused) {
        sf::RectangleShape cursor;
        cursor.setSize(sf::Vector2f(2, nameInput.getSize().y - 10));
        cursor.setPosition(nameText.getPosition().x + nameText.getLocalBounds().width + 2, nameInput.getPosition().y + 5);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    window.draw(passwordInput);
    window.draw(passwordLabel);
    window.draw(passwordText);

    if (isPasswordFocused) {
        sf::RectangleShape cursor;
        cursor.setSize(sf::Vector2f(2, passwordInput.getSize().y - 10));
        cursor.setPosition(passwordText.getPosition().x + passwordText.getLocalBounds().width + 2, passwordInput.getPosition().y + 5);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    window.draw(maxPlayersInput);
    window.draw(maxPlayersLabel);
    window.draw(maxPlayersText);

    if (isMaxPlayersFocused) {
        sf::RectangleShape cursor;
        cursor.setSize(sf::Vector2f(2, maxPlayersInput.getSize().y - 10));
        cursor.setPosition(maxPlayersText.getPosition().x + maxPlayersText.getLocalBounds().width + 2, maxPlayersInput.getPosition().y + 5);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    window.draw(submitButton);
    if (isLoading) {
        loadingSpinner.setPosition(
            submitButton.getPosition().x + submitButton.getSize().x / 2,
            submitButton.getPosition().y + submitButton.getSize().y / 2
        );
        window.draw(loadingSpinner);
    } else {
        window.draw(submitText);
    }
    
    window.draw(backButton);
    window.draw(backText);
}

void Menu::renderRoomLobby() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;

    lobbybackButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    lobbybackButton.setPosition(windowWidth * 0.3f - buttonWidth/2, windowHeight * 0.8f);
    lobbybackButton.setFillColor(sf::Color(100, 100, 100));
    window.draw(lobbybackButton);

    backText.setFont(robotoFont);
    backText.setString("Retour");
    backText.setCharacterSize(20);
    backText.setPosition(
        lobbybackButton.getPosition().x + (lobbybackButton.getSize().x - backText.getGlobalBounds().width) / 2,
        lobbybackButton.getPosition().y + (lobbybackButton.getSize().y - backText.getGlobalBounds().height) / 2
    );
    window.draw(backText);
    bool isFirstPlayer = !lobbyPlayers.empty() && 
                        std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0];
    
    if (isFirstPlayer) {
        lobbystartButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        lobbystartButton.setPosition(windowWidth * 0.7f - buttonWidth/2, windowHeight * 0.8f);
        lobbystartButton.setFillColor(sf::Color(0, 150, 0));
        window.draw(lobbystartButton);
        
        sf::Text startText;
        startText.setFont(robotoFont);
        startText.setString("Lancer la partie");
        startText.setCharacterSize(20);
        startText.setPosition(
            lobbystartButton.getPosition().x + (buttonWidth - startText.getGlobalBounds().width)/2,
            lobbystartButton.getPosition().y + (buttonHeight - startText.getGlobalBounds().height)/2
        );
        window.draw(startText);
    }

    // Room title
    sf::Text roomTitle;
    roomTitle.setFont(robotoFont);
    roomTitle.setString("Room #" + currentRoomId);
    roomTitle.setCharacterSize(30);
    roomTitle.setPosition(
        (windowWidth - roomTitle.getGlobalBounds().width) / 2,
        windowHeight * 0.2f
    );
    
    // Players list avec indication du host
    sf::Text playersList;
    playersList.setFont(robotoFont);
    playersList.setString("Joueurs dans la room:");
    playersList.setCharacterSize(20);
    playersList.setPosition(
        (windowWidth - playersList.getGlobalBounds().width) / 2,
        windowHeight * 0.4f
    );
    
    float playerListY = windowHeight * 0.4f + 40;
    for (size_t i = 0; i < lobbyPlayers.size(); ++i) {
        sf::Text playerText;
        playerText.setFont(robotoFont);
        std::string playerLabel = "Joueur #" + lobbyPlayers[i];
        if (i == 0) {
            playerLabel += " (Host)";
        }
        playerText.setString(playerLabel);
        playerText.setCharacterSize(20);
        playerText.setPosition(
            (windowWidth - playerText.getGlobalBounds().width) / 2,
            playerListY
        );
        window.draw(playerText);
        playerListY += 30;
    }
    
    window.draw(roomTitle);
    window.draw(playersList);
    renderChat();

    // Initialisation des textes des boutons de difficulté
    difficultyEasyText.setFont(robotoFont);
    difficultyEasyText.setString("Facile");
    difficultyEasyText.setCharacterSize(20);
    difficultyEasyText.setFillColor(sf::Color::White);
    difficultyEasyText.setPosition(
        difficultyEasyButton.getPosition().x + (difficultyEasyButton.getSize().x - difficultyEasyText.getLocalBounds().width) / 2,
        difficultyEasyButton.getPosition().y + (difficultyEasyButton.getSize().y - difficultyEasyText.getLocalBounds().height) / 2
    );

    difficultyMediumText.setFont(robotoFont);
    difficultyMediumText.setString("Moyen");
    difficultyMediumText.setCharacterSize(20);
    difficultyMediumText.setFillColor(sf::Color::White);
    difficultyMediumText.setPosition(
        difficultyMediumButton.getPosition().x + (difficultyMediumButton.getSize().x - difficultyMediumText.getLocalBounds().width) / 2,
        difficultyMediumButton.getPosition().y + (difficultyMediumButton.getSize().y - difficultyMediumText.getLocalBounds().height) / 2
    );

    difficultyHardText.setFont(robotoFont);
    difficultyHardText.setString("Difficile");
    difficultyHardText.setCharacterSize(20);
    difficultyHardText.setFillColor(sf::Color::White);
    difficultyHardText.setPosition(
        difficultyHardButton.getPosition().x + (difficultyHardButton.getSize().x - difficultyHardText.getLocalBounds().width) / 2,
        difficultyHardButton.getPosition().y + (difficultyHardButton.getSize().y - difficultyHardText.getLocalBounds().height) / 2
    );

    if (!lobbyPlayers.empty() && std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0]) {
        window.draw(difficultyEasyButton);
        window.draw(difficultyEasyText);
        window.draw(difficultyMediumButton);
        window.draw(difficultyMediumText);
        window.draw(difficultyHardButton);
        window.draw(difficultyHardText);
    }
    
    if (showCustomCursor) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        cursorSprite.setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        window.draw(cursorSprite);
    }
}

void Menu::createRoomForm() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Bouton de création dans le menu des rooms
    float createButtonWidth = windowWidth * 0.12f;
    float createButtonHeight = windowHeight * 0.05f;
    
    createRoomButton.setSize(sf::Vector2f(createButtonWidth, createButtonHeight));
    createRoomButton.setPosition(windowWidth * 0.83f, windowHeight * 0.05f);
    createRoomButton.setFillColor(sf::Color(70, 70, 70));
    createRoomButton.setOutlineThickness(2);
    createRoomButton.setOutlineColor(sf::Color(100, 100, 100));
    
    createRoomText.setFont(robotoFont);
    createRoomText.setString("Creer");
    createRoomText.setCharacterSize(windowHeight * 0.03f);
    
    sf::FloatRect createBounds = createRoomText.getLocalBounds();
    createRoomText.setPosition(
        windowWidth * 0.83f + (createButtonWidth - createBounds.width) / 2,
        windowHeight * 0.05f + (createButtonHeight - createBounds.height) / 2
    );
    
    // Formulaire
    float inputWidth = windowWidth * 0.3f;
    float inputHeight = windowHeight * 0.05f;
    float centerX = (windowWidth - inputWidth) / 2;
    float startY = windowHeight * 0.2f;
    
    // Champ nom
    nameInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    nameInput.setPosition(centerX, startY);
    nameInput.setFillColor(sf::Color(50, 50, 50));
    nameInput.setOutlineThickness(2);
    nameInput.setOutlineColor(sf::Color(150, 150, 150));
    
    nameLabel.setFont(robotoFont);
    nameLabel.setString("Nom de la room:");
    nameLabel.setCharacterSize(windowHeight * 0.03f);
    nameLabel.setPosition(centerX, startY - 30);
    
    nameText.setFont(robotoFont);
    nameText.setCharacterSize(windowHeight * 0.03f);
    nameText.setPosition(centerX + 10, startY + 5);
    
    // Champ mot de passe
    passwordInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    passwordInput.setPosition(centerX, startY + 100);
    passwordInput.setFillColor(sf::Color(50, 50, 50));
    passwordInput.setOutlineThickness(2);
    passwordInput.setOutlineColor(sf::Color(150, 150, 150));
    
    passwordLabel.setFont(robotoFont);
    passwordLabel.setString("Mot de passe (optionnel):");
    passwordLabel.setCharacterSize(windowHeight * 0.03f);
    passwordLabel.setPosition(centerX, startY + 70);
    
    passwordText.setFont(robotoFont);
    passwordText.setCharacterSize(windowHeight * 0.03f);
    passwordText.setPosition(centerX + 10, startY + 105);
    
    // Champ nombre max de joueurs
    maxPlayersInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    maxPlayersInput.setPosition(centerX, startY + 200);
    maxPlayersInput.setFillColor(sf::Color(50, 50, 50));
    maxPlayersInput.setOutlineThickness(2);
    maxPlayersInput.setOutlineColor(sf::Color(150, 150, 150));
    
    maxPlayersLabel.setFont(robotoFont);
    maxPlayersLabel.setString("Nombre maximum de joueurs:");
    maxPlayersLabel.setCharacterSize(windowHeight * 0.03f);
    maxPlayersLabel.setPosition(centerX, startY + 170);
    
    maxPlayersText.setFont(robotoFont);
    maxPlayersText.setCharacterSize(windowHeight * 0.03f);
    maxPlayersText.setPosition(centerX + 10, startY + 205);

    // Ajoutez la difficulté à la commande de création de room
    std::string command = "102 " + nameString + " " + passwordString + " " + maxPlayersString;
    networkManager.sendTCP(command);
    
    // Bouton de soumission
    submitButton.setSize(sf::Vector2f(inputWidth * 0.5f, inputHeight));
    submitButton.setPosition(centerX + inputWidth * 0.25f, startY + 300);
    submitButton.setFillColor(sf::Color(70, 70, 70));
    
    submitText.setFont(robotoFont);
    submitText.setString("Creer la room");
    submitText.setCharacterSize(windowHeight * 0.03f);
    sf::FloatRect submitBounds = submitText.getLocalBounds();
    submitText.setPosition(
        centerX + inputWidth * 0.5f - submitBounds.width / 2,
        startY + 300 + (inputHeight - submitBounds.height) / 2
    );
    
    isNameFocused = false;
    isPasswordFocused = false;
    isMaxPlayersFocused = false;
    nameString = "";
    passwordString = "";
    maxPlayersString = "";
}

void Menu::resizeCreateRoomMenu() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;

    // Dimensions des éléments proportionnelles à la taille de la fenêtre
    float inputWidth = windowWidth * 0.3f;
    float inputHeight = windowHeight * 0.05f;
    float centerX = (windowWidth - inputWidth) / 2;  // Centré horizontalement
    float startY = windowHeight * 0.15f;
    float spacing = windowHeight * 0.1f;

    // Champ nom
    nameInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    nameInput.setPosition(centerX, startY);
    nameInput.setFillColor(sf::Color(50, 50, 50));
    nameInput.setOutlineThickness(2);
    nameInput.setOutlineColor(sf::Color(150, 150, 150));

    nameLabel.setFont(robotoFont);
    nameLabel.setString("Nom de la room:");
    nameLabel.setCharacterSize(windowHeight * 0.025f);
    nameLabel.setPosition(centerX, startY - 30);
    nameLabel.setFillColor(sf::Color::White);

    nameText.setFont(robotoFont);
    nameText.setCharacterSize(windowHeight * 0.025f);
    nameText.setPosition(centerX + 10, startY + 5);
    nameText.setFillColor(sf::Color::White);

    // Champ mot de passe
    passwordInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    passwordInput.setPosition(centerX, startY + spacing);
    passwordInput.setFillColor(sf::Color(50, 50, 50));
    passwordInput.setOutlineThickness(2);
    passwordInput.setOutlineColor(sf::Color(150, 150, 150));

    passwordLabel.setFont(robotoFont);
    passwordLabel.setString("Mot de passe (optionnel):");
    passwordLabel.setCharacterSize(windowHeight * 0.025f);
    passwordLabel.setPosition(centerX, startY + spacing - 30);
    passwordLabel.setFillColor(sf::Color::White);

    passwordText.setFont(robotoFont);
    passwordText.setCharacterSize(windowHeight * 0.025f);
    passwordText.setPosition(centerX + 10, startY + spacing + 5);
    passwordText.setFillColor(sf::Color::White);

    // Champ nombre max de joueurs
    maxPlayersInput.setSize(sf::Vector2f(inputWidth, inputHeight));
    maxPlayersInput.setPosition(centerX, startY + spacing * 2);
    maxPlayersInput.setFillColor(sf::Color(50, 50, 50));
    maxPlayersInput.setOutlineThickness(2);
    maxPlayersInput.setOutlineColor(sf::Color(150, 150, 150));

    maxPlayersLabel.setFont(robotoFont);
    maxPlayersLabel.setString("Nombre maximum de joueurs:");
    maxPlayersLabel.setCharacterSize(windowHeight * 0.025f);
    maxPlayersLabel.setPosition(centerX, startY + spacing * 2 - 30);
    maxPlayersLabel.setFillColor(sf::Color::White);

    maxPlayersText.setFont(robotoFont);
    maxPlayersText.setCharacterSize(windowHeight * 0.025f);
    maxPlayersText.setPosition(centerX + 10, startY + spacing * 2 + 5);
    maxPlayersText.setFillColor(sf::Color::White);

    // Bouton de création
    float submitButtonWidth = inputWidth * 0.5f;
    submitButton.setSize(sf::Vector2f(submitButtonWidth, inputHeight));
    submitButton.setPosition(centerX + (inputWidth - submitButtonWidth) / 2, startY + spacing * 3);
    submitButton.setFillColor(sf::Color(70, 70, 70));

    submitText.setFont(robotoFont);
    submitText.setString("Creer la room");
    submitText.setCharacterSize(windowHeight * 0.025f);
    submitText.setFillColor(sf::Color::White);
    centerTextInButton(submitText, submitButton);

    // Bouton retour
    float backButtonWidth = windowWidth * 0.12f;
    float backButtonHeight = windowHeight * 0.05f;
    backButton.setSize(sf::Vector2f(backButtonWidth, backButtonHeight));
    backButton.setPosition(windowWidth * 0.05f, windowHeight * 0.9f);
    backButton.setFillColor(sf::Color(70, 70, 70));

    backText.setFont(robotoFont);
    backText.setString("Retour");
    backText.setCharacterSize(windowHeight * 0.025f);
    backText.setFillColor(sf::Color::White);
    centerTextInButton(backText, backButton);
}
