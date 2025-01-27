/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Menu
*/

#include "../include/Menu/Menu.hpp"
#include "../include/Parser/jsonParser.hpp"
#include <iostream>
#include <algorithm>
#include <thread>

Menu::StateChangeCallback Menu::globalStateChangeCallback = nullptr;

Menu::Menu(sf::RenderWindow& window, World* gameWorld, std::string ip, std::string port) : 
    window(window),
    currentState(MenuState::MAIN_MENU),
    previousState(MenuState::MAIN_MENU),
    showCustomCursor(true),
    networkManager(ip, port),
    world(gameWorld) {
    
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    loadFonts();
    
    createButtons();
    createPauseMenu();
    createGameOverMenu();
    createWinMenu();
    initializeRooms();
    updateCursor();
    createRoomForm();
    
    isSettingsMenuOpen = false;
    isWaitingForKey = false;
    
    // Touches par défaut
    keyBindings.moveUp = sf::Keyboard::Z;
    keyBindings.moveDown = sf::Keyboard::S;
    keyBindings.moveLeft = sf::Keyboard::Q;
    keyBindings.moveRight = sf::Keyboard::D;
    keyBindings.shoot = sf::Keyboard::Space;
    
    // Configuration du bouton paramètres
    float settingsButtonWidth = windowWidth * 0.12f;
    float settingsButtonHeight = windowHeight * 0.05f;
    
    settingsButton.setSize(sf::Vector2f(settingsButtonWidth, settingsButtonHeight));
    settingsButton.setPosition(windowWidth * 0.05f, windowHeight * 0.10f);
    settingsButton.setFillColor(isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70));
    
    settingsText.setFont(font);
    settingsText.setString("Parametres");
    settingsText.setCharacterSize(windowHeight * 0.03f);
    
    sf::FloatRect settingsBounds = settingsText.getLocalBounds();
    settingsText.setPosition(
        windowWidth * 0.83f + (settingsButtonWidth - settingsBounds.width) / 2,
        windowHeight * 0.25f + (settingsButtonHeight - settingsBounds.height) / 2
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

    // Initialisation des textes du lobby
    roomTitle.setFont(font);
    roomTitle.setCharacterSize(windowHeight * 0.04f);
    roomTitle.setString("Lobby");
    
    playersList.setFont(font);
    playersList.setCharacterSize(windowHeight * 0.03f);
    playersList.setString("Joueurs :");
    
    startText.setFont(font);
    startText.setCharacterSize(windowHeight * 0.03f);
    startText.setString("Démarrer");
    
    // Positionnement des textes
    roomTitle.setPosition(windowWidth * 0.05f, windowHeight * 0.1f);
    playersList.setPosition(windowWidth * 0.05f, windowHeight * 0.2f);
    startText.setPosition(windowWidth * 0.05f, windowHeight * 0.8f);

    isDaltonianMode = false;
    activeDifficultyButton = &difficultyEasyButton; // Puisque EASY est la difficulté par défaut
    difficultyEasyButton.setFillColor(sf::Color(120, 120, 120));

    sliderWidth = windowWidth * 0.2f;
    sliderHeight = windowHeight * 0.01f;
    sliderX = windowWidth * 0.5f;
    sliderY = windowHeight * 0.8f;

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

    
    createScoreboard();
    updateVolumeHandlePosition();
    createScoreboard();
    updateScoreboard();
    createChat();

    // Initialisation des éléments du prompt de mot de passe
    passwordPromptLabel.setFont(font);
    passwordPromptLabel.setCharacterSize(window.getSize().y * 0.03f);
    passwordPromptLabel.setFillColor(sf::Color::White);
    
    passwordPromptText.setFont(font);
    passwordPromptText.setCharacterSize(window.getSize().y * 0.03f);
    passwordPromptText.setFillColor(sf::Color::White);
    
    passwordPromptSubmitText.setFont(font);
    passwordPromptSubmitText.setCharacterSize(window.getSize().y * 0.03f);
    passwordPromptSubmitText.setFillColor(sf::Color::White);
    
    passwordPromptBackText.setFont(font);
    passwordPromptBackText.setCharacterSize(window.getSize().y * 0.03f);
    passwordPromptBackText.setFillColor(sf::Color::White);
    
    isPasswordPromptVisible = false;

    accessibilityText.setFont(font);
    accessibilityText.setString("Accessibilite");
    accessibilityText.setCharacterSize(windowHeight * 0.03f);
    accessibilityText.setFillColor(sf::Color::White);

    // Centrage initial du texte dans le bouton
    sf::FloatRect accessibilityBounds = accessibilityText.getLocalBounds();
    accessibilityText.setPosition(
        windowWidth * 0.05f + (windowWidth * 0.12f - accessibilityBounds.width) / 2,
        windowHeight * 0.05f + (windowHeight * 0.05f - accessibilityBounds.height) / 2
    );

    float matchmakingButtonWidth = window.getSize().x * 0.15f;
    float matchmakingButtonHeight = window.getSize().y * 0.05f;
    matchmakingButton.setSize(sf::Vector2f(matchmakingButtonWidth, matchmakingButtonHeight));
    matchmakingButton.setPosition(window.getSize().x * 0.5f - matchmakingButtonWidth / 2, window.getSize().y * 0.9f);
    matchmakingButton.setFillColor(sf::Color(70, 70, 70));
    matchmakingButton.setOutlineThickness(2);
    matchmakingButton.setOutlineColor(sf::Color(100, 100, 100));

    matchmakingText.setFont(robotoFont);
    matchmakingText.setString("Matchmaking");
    matchmakingText.setCharacterSize(window.getSize().y * 0.025f);
    matchmakingText.setFillColor(sf::Color::White);

    // Centrer le texte dans le bouton
    sf::FloatRect matchmakingBounds = matchmakingText.getLocalBounds();
    matchmakingText.setPosition(
        matchmakingButton.getPosition().x + (matchmakingButtonWidth - matchmakingBounds.width) / 2,
        matchmakingButton.getPosition().y + (matchmakingButtonHeight - matchmakingBounds.height) / 2
    );
}


void Menu::render() {
    switch (currentState) {
        case MenuState::MAIN_MENU:
            renderMainMenu();
            break;
        case MenuState::ROOM_SELECT:
            renderRoomSelect();
            renderPasswordPrompt();
            renderChat();
            window.draw(settingsButton);
            window.draw(settingsText);
            if (isSettingsMenuOpen) {
                renderSettingsMenu();
            }
            break;
        case MenuState::ROOM_LOBBY:
            renderRoomLobby();
            renderChat();
            break;
        case MenuState::GAME_OVER:
            renderGameOver();
            break;
        case MenuState::ROOM_CREATE:
            renderRoomCreate();
            break;
        case MenuState::WIN:
            renderWinScreen();
            break;
        default:
            break;
    }

    if (currentState == MenuState::ROOM_LOBBY && 
        !lobbyPlayers.empty() && 
        std::to_string(networkManager.getPlayerId()) == lobbyPlayers[0]) {
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


void Menu::handleEvent(const sf::Event& event) {
    if (isSettingsMenuOpen) {
        handleSettingsEvents(event);
        return;
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (settingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isSettingsMenuOpen = true;
            return;
        }
    }

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        if (isSettingsMenuOpen) {
            isSettingsMenuOpen = false;
            return;
        }
    }
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::TextEntered && isSearchBarFocused) {
        if (event.text.unicode == '\b' && !searchString.empty()) {
            searchString.pop_back();
        } else if (event.text.unicode < 128 && event.text.unicode != '\b') {
            searchString += static_cast<char>(event.text.unicode);
        }
        searchText.setString(searchString);
    }

    switch (currentState) {
        case MenuState::MAIN_MENU:
            handleMainMenuEvents(event);
            break;
        case MenuState::ROOM_SELECT:
            handleRoomSelectEvents(event);
            break;
        case MenuState::GAME_OVER:
            handleGameOverEvents(event);
            break;
        case MenuState::ROOM_CREATE:
            handleRoomCreateEvents(event);
            break;
        case MenuState::ROOM_LOBBY:
            handleRoomLobbyEvents(event);
            break;
        case MenuState::WIN:
            handleWinEvents(event);
            break;
        default:
            break;
    }

    if (!searchBar.getGlobalBounds().contains(mousePos.x, mousePos.y) &&
        isSearchBarFocused && event.type == sf::Event::MouseButtonPressed) {
        isSearchBarFocused = false;
    } 

    handlePasswordEvents(event);

    if (networkManager.hasMessages()) {
        std::string message = networkManager.getNextMessage();
        if (!message.empty()) {
            if (!message.empty() && message.find("$125") != std::string::npos) {
                //std::cout << "Leaderboard reçu dans le handleEvent: " << message << std::endl;
                try {
                    JsonParser parser(message);
                    auto leaderboard = parser.parseLeaderboard();
                    std::vector<std::pair<std::string, int>> scores;
                    
                    for (const auto& entry : leaderboard) {
                        scores.push_back({entry.name, entry.score});
                    }
                    
                    updateScoreboardEntries(scores);
                } catch (const std::exception& e) {
                    std::cerr << "Erreur parsing leaderboard: " << e.what() << std::endl;
                }
            }
            else if (message[0] == '{') {
                if (currentState == MenuState::ROOM_LOBBY) {
                    updateLobbyPlayers(message);
                    //std::cout << message << std::endl;
                } else {
                    updateRooms(message);
                }
            }
        }
    }

    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        sf::Color defaultColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);
        sf::Color hoverColor = isHighContrastMode ? sf::Color(230, 230, 230) : sf::Color(90, 90, 90);
        
        if (settingsButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            settingsButton.setFillColor(hoverColor);
        } else {
            settingsButton.setFillColor(defaultColor);
        }
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
        sf::FloatRect handleBounds = volumeSliderHandle.getGlobalBounds();
        if (handleBounds.contains(mousePos.x, mousePos.y)) {
            isDraggingVolume = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDraggingVolume = false;
    }

    if (event.type == sf::Event::MouseMoved && isDraggingVolume) {
        float mouseX = event.mouseMove.x;
        float sliderStart = volumeSlider.getPosition().x;
        float sliderEnd = sliderStart + volumeSlider.getSize().x;
        
        float newVolume = (mouseX - sliderStart) / volumeSlider.getSize().x;
        newVolume = std::max(0.0f, std::min(1.0f, newVolume));
        
        currentVolume = newVolume;
        updateVolumeHandlePosition();
        
        // Mettre à jour le volume du SoundManager
        SoundManager::setGlobalVolume(currentVolume * 100.0f);
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (daltonianButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isDaltonianMode = !isDaltonianMode;
            
            // Appliquer le filtre de couleur
            if (isDaltonianMode) {
                // Deutéranopie (rouge-vert)
                soloButton.setFillColor(sf::Color(0, 170, 230));     // Bleu clair
                multiButton.setFillColor(sf::Color(0, 170, 230));
                settingsButton.setFillColor(sf::Color(0, 170, 230));
                
                // Pour les textes
                soloText.setFillColor(sf::Color::Yellow);
                multiText.setFillColor(sf::Color::Yellow);
                quitText.setFillColor(sf::Color::Yellow);
                settingsText.setFillColor(sf::Color::Yellow);
            } else {
                // Retour aux couleurs normales
                sf::Color defaultColor = isHighContrastMode ? sf::Color::White : sf::Color(100, 100, 100);
                
                soloButton.setFillColor(defaultColor);
                multiButton.setFillColor(defaultColor);
                settingsButton.setFillColor(defaultColor);
                
                // Pour les textes
                sf::Color textColor = isHighContrastMode ? sf::Color::Black : sf::Color::White;
                soloText.setFillColor(textColor);
                multiText.setFillColor(textColor);
                quitText.setFillColor(textColor);
                settingsText.setFillColor(textColor);
            }
        }
    }

    if (event.type == sf::Event::Resized) {
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
        updateVolumeSliderPositions();
    }
}


void Menu::handleResize() {
    if (currentState == MenuState::ROOM_CREATE) {
        resizeCreateRoomMenu();
        return;
    }
    if (currentState == MenuState::MAIN_MENU) {
        resizeMainMenu();
        return;
    }
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    float soloY = windowHeight * 0.3f;
    float multiY = windowHeight * 0.5f;
    float quitY = windowHeight * 0.7f;

    chatBox.setSize(sf::Vector2f(windowWidth * 0.3f, windowHeight * 0.3f));
    chatBox.setPosition(60, windowHeight * 0.5f);
    soloButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    multiButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    
    chatInput.setSize(sf::Vector2f(windowWidth * 0.3f, 30));
    chatInput.setPosition(60, windowHeight * 0.5f + chatBox.getSize().y + 5);
    
    soloButton.setPosition(centerX, soloY);
    multiButton.setPosition(centerX, multiY);
    
    // Mise à jour de la taille du texte
    float textSize = windowHeight * 0.04f;
    soloText.setCharacterSize(textSize);
    multiText.setCharacterSize(textSize);
    quitText.setCharacterSize(textSize);
    
    // Centrage du texte dans les boutons
    centerTextInButton(soloText, soloButton);
    centerTextInButton(multiText, multiButton);

    // Mise à jour du bouton d'accessibilité
    float accessibilityButtonWidth = windowWidth * 0.12f;
    float accessibilityButtonHeight = windowHeight * 0.05f;
    
    accessibilityButton.setSize(sf::Vector2f(accessibilityButtonWidth, accessibilityButtonHeight));
    accessibilityButton.setPosition(windowWidth * 0.05f, windowHeight * 0.05f);
    
    accessibilityText.setFont(font);
    accessibilityText.setString("Accessibilite");
    accessibilityText.setCharacterSize(windowHeight * 0.03f);
    accessibilityText.setFillColor(sf::Color::White);

    // Centrage du texte dans le bouton
    sf::FloatRect accessibilityBounds = accessibilityText.getLocalBounds();
    accessibilityText.setPosition(
        windowWidth * 0.05f + (accessibilityButtonWidth - accessibilityBounds.width) / 2,
        windowHeight * 0.05f + (accessibilityButtonHeight - accessibilityBounds.height) / 2
    );

    // Mise à jour du bouton paramètres
    float settingsButtonWidth = windowWidth * 0.12f;
    float settingsButtonHeight = windowHeight * 0.05f;
    
    settingsButton.setSize(sf::Vector2f(settingsButtonWidth, settingsButtonHeight));
    settingsButton.setPosition(windowWidth * 0.05f, windowHeight * 0.25f);
    settingsButton.setFillColor(sf::Color(70, 70, 70));
    
    settingsText.setCharacterSize(windowHeight * 0.03f);
    centerTextInButton(settingsText, settingsButton);

    // Continuer avec le reste des mises à jour existantes
    // Mise à jour de la barre de recherche
    float searchBarWidth = windowWidth * 0.3f;
    float searchBarHeight = windowHeight * 0.05f;
    searchBar.setSize(sf::Vector2f(searchBarWidth, searchBarHeight));
    searchBar.setPosition(windowWidth * 0.35f, windowHeight * 0.05f);
    searchText.setPosition(
        windowWidth * 0.35f + 10,
        windowHeight * 0.05f + (searchBarHeight - searchText.getCharacterSize()) / 2
    );
    searchText.setFillColor(sf::Color::Black);
    searchBar.setFillColor(sf::Color::White);

    chatInputText.setPosition(65, chatInput.getPosition().y + 5);
    
    // Le bouton toggle reste en haut à gauche
    float chatToggleWidth = windowWidth * 0.05f;  // Augmentation de la taille
    float chatToggleHeight = windowHeight * 0.05f;  // Augmentation de la taille
    chatToggleButton.setSize(sf::Vector2f(chatToggleWidth, chatToggleHeight));
    chatToggleButton.setPosition(10, 10);
    chatToggleText.setPosition(
        chatToggleButton.getPosition().x + (chatToggleWidth - chatToggleText.getGlobalBounds().width) / 2,
        chatToggleButton.getPosition().y + (chatToggleHeight - chatToggleText.getGlobalBounds().height) / 2
    );

    // Mise à jour du bouton refresh
    float refreshButtonWidth = windowWidth * 0.12f;
    float refreshButtonHeight = windowHeight * 0.05f;
    refreshButton.setSize(sf::Vector2f(refreshButtonWidth, refreshButtonHeight));
    refreshButton.setPosition(windowWidth * 0.83f, windowHeight * 0.9f);
    
    sf::FloatRect refreshBounds = refreshText.getLocalBounds();
    refreshText.setPosition(
        windowWidth * 0.83f + (refreshButtonWidth - refreshBounds.width) / 2,
        windowHeight * 0.9f + (refreshButtonHeight - refreshBounds.height) / 2
    );

    // Mise à jour du bouton create
    createRoomButton.setSize(sf::Vector2f(refreshButtonWidth, refreshButtonHeight));
    createRoomButton.setPosition(windowWidth * 0.83f, windowHeight * 0.05f);
    
    sf::FloatRect createBounds = createRoomText.getLocalBounds();
    createRoomText.setPosition(
        windowWidth * 0.83f + (refreshButtonWidth - createBounds.width) / 2,
        windowHeight * 0.05f + (refreshButtonHeight - createBounds.height) / 2
    );

    // Mise à jour du bouton retour
    float backButtonWidth = windowWidth * 0.12f;
    float backButtonHeight = windowHeight * 0.05f;
    backButton.setSize(sf::Vector2f(backButtonWidth, backButtonHeight));
    backButton.setPosition(windowWidth * 0.05f, windowHeight * 0.9f);
    
    sf::FloatRect backBounds = backText.getLocalBounds();
    backText.setPosition(
        windowWidth * 0.05f + (backButtonWidth - backBounds.width) / 2,
        windowHeight * 0.9f + (backButtonHeight - backBounds.height) / 2
    );

    // Mise à jour des rooms
    float roomButtonWidth = windowWidth * 0.5f;
    float roomButtonHeight = windowHeight * 0.08f;
    float roomStartY = windowHeight * 0.2f;
    float roomSpacing = windowHeight * 0.1f;
    float joinButtonWidth = windowWidth * 0.1f;
    float joinButtonHeight = roomButtonHeight * 0.8f;

    for (size_t i = 0; i < roomButtons.size(); ++i) {
        roomButtons[i].setSize(sf::Vector2f(roomButtonWidth, roomButtonHeight));
        roomButtons[i].setPosition((windowWidth - roomButtonWidth) / 2, roomStartY + (i * roomSpacing));
        roomButtons[i].setFillColor(sf::Color(120, 120, 120));

        // Mise à jour des boutons "Rejoindre"
        if (i < joinButtons.size()) {
            joinButtons[i].setSize(sf::Vector2f(joinButtonWidth, joinButtonHeight));
            joinButtons[i].setPosition(
                (windowWidth + roomButtonWidth) / 2 - joinButtonWidth,
                roomStartY + (i * roomSpacing) + (roomButtonHeight - joinButtonHeight) / 2
            );

            // Mise à jour du texte "Rejoindre"
            if (i < joinButtonTexts.size()) {
                sf::FloatRect joinTextBounds = joinButtonTexts[i].getLocalBounds();
                joinButtonTexts[i].setPosition(
                    joinButtons[i].getPosition().x + (joinButtonWidth - joinTextBounds.width) / 2,
                    joinButtons[i].getPosition().y + (joinButtonHeight - joinTextBounds.height) / 2
                );
            }
        }

        

        // Mise à jour du texte des rooms
        if (i * 2 < roomTexts.size()) {
            float padding = 10.0f;
            float maxNameWidth = roomButtonWidth * 0.6f; // Limite la largeur du nom à 60% du bouton
            
            // Position du nom de la room
            roomTexts[i * 2].setPosition(
                (windowWidth - roomButtonWidth) / 2 + padding,
                roomStartY + (i * roomSpacing) + (roomButtonHeight - roomTexts[i * 2].getCharacterSize()) / 2
            );
            
            // Tronquer le texte si nécessaire
            std::string roomName = roomTexts[i * 2].getString();
            while (roomTexts[i * 2].getGlobalBounds().width > maxNameWidth && roomName.length() > 3) {
                roomName = roomName.substr(0, roomName.length() - 4) + "...";
                roomTexts[i * 2].setString(roomName);
            }
            
            // Position du nombre de joueurs
            if (i * 2 + 1 < roomTexts.size()) {
                float joinButtonX = (windowWidth + roomButtonWidth) / 2 - joinButtonWidth;
                float playerCountX = joinButtonX - roomTexts[i * 2 + 1].getGlobalBounds().width - 40;
                roomTexts[i * 2 + 1].setPosition(
                    playerCountX,
                    roomStartY + (i * roomSpacing) + (roomButtonHeight - roomTexts[i * 2 + 1].getCharacterSize()) / 2
                );
            }
        }
    }

    // Appel des autres mises à jour
    updateRoomListPositions();
    updatePauseMenuPositions();
    updateGameOverPositions();
    updateWinScreenPositions();
    
    // Mise à jour des positions des éléments
    updateRoomElements();
    
    // Maintien des styles d'accessibilité
    updateAccessibilityStyles();

    updateDaltonianMode();
    applyDaltonianColors();

    // Mise à jour des boutons de difficulté
    float difficultyButtonWidth = windowWidth * 0.15f;
    float difficultyButtonHeight = windowHeight * 0.05f;
    float difficultyY = windowHeight * 0.3f;
    float difficultySpacing = windowWidth * 0.05f;

    // Bouton Easy
    difficultyEasyButton.setSize(sf::Vector2f(difficultyButtonWidth, difficultyButtonHeight));
    difficultyEasyButton.setPosition(windowWidth * 0.2f, difficultyY);
    difficultyEasyButton.setFillColor(sf::Color(100, 100, 100));
    centerTextInButton(difficultyEasyText, difficultyEasyButton);

    // Bouton Medium
    difficultyMediumButton.setSize(sf::Vector2f(difficultyButtonWidth, difficultyButtonHeight));
    difficultyMediumButton.setPosition(windowWidth * 0.4f, difficultyY);
    difficultyMediumButton.setFillColor(sf::Color(100, 100, 100));
    centerTextInButton(difficultyMediumText, difficultyMediumButton);

    // Bouton Hard
    difficultyHardButton.setSize(sf::Vector2f(difficultyButtonWidth, difficultyButtonHeight));
    difficultyHardButton.setPosition(windowWidth * 0.6f, difficultyY);
    difficultyHardButton.setFillColor(sf::Color(100, 100, 100));
    centerTextInButton(difficultyHardText, difficultyHardButton);

    // Mise à jour de la taille du texte des boutons de difficulté
    float difficultyTextSize = windowHeight * 0.03f;
    difficultyEasyText.setCharacterSize(difficultyTextSize);
    difficultyMediumText.setCharacterSize(difficultyTextSize);
    difficultyHardText.setCharacterSize(difficultyTextSize);

    // Mise à jour du scoreboard
    if (isScoreboardVisible) {
        createScoreboard();
        //updateScoreboard();
    }

        // Mise à jour des messages
    updateChatMessages();
    centerAllButtonTexts();

    // Mise à jour du bouton matchmaking
    float matchmakingButtonWidth = windowWidth * 0.12f;
    float matchmakingButtonHeight = windowHeight * 0.05f;
    matchmakingButton.setSize(sf::Vector2f(matchmakingButtonWidth, matchmakingButtonHeight));
    matchmakingButton.setPosition(windowWidth * 0.83f, windowHeight * 0.15f);

    // Mise à jour du texte du bouton matchmaking
    sf::FloatRect matchmakingBounds = matchmakingText.getLocalBounds();
    matchmakingText.setPosition(
        windowWidth * 0.83f + (matchmakingButtonWidth - matchmakingBounds.width) / 2,
        windowHeight * 0.15f + (matchmakingButtonHeight - matchmakingBounds.height) / 2
    );
}

void Menu::centerTextInButton(sf::Text& text, const sf::RectangleShape& button) {
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setPosition(
        button.getPosition().x + (button.getSize().x - textBounds.width) / 2,
        button.getPosition().y + (button.getSize().y - textBounds.height) / 2
    );
}


void Menu::setState(MenuState state) {
    previousState = currentState;
    currentState = state;
    updateCursor();
    
    if (state == MenuState::ROOM_SELECT) {
        networkManager.requestLeaderboard();  // Demander le leaderboard une seule fois
    }

    if (state == MenuState::ROOM_CREATE) {
        isNameFocused = true;
        isPasswordFocused = false;
        isMaxPlayersFocused = false;
        
        nameInput.setOutlineColor(sf::Color(200, 200, 200));
        passwordInput.setOutlineColor(sf::Color(150, 150, 150));
        maxPlayersInput.setOutlineColor(sf::Color(150, 150, 150));
    }
    
    if (stateChangeCallback) {
        stateChangeCallback(state);
    }
    if (isDaltonianMode) {
        applyDaltonianColors();
    }
}

void Menu::updateCursor() {
    if (!cursorTexture.loadFromFile("client/sprites/cursor.png")) {
        throw std::runtime_error("Impossible de charger la texture du curseur");
    }
    cursorSprite.setTexture(cursorTexture);
    cursorSprite.setScale(0.01f, 0.01f);

    sf::Vector2u textureSize = cursorTexture.getSize();
    cursorSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    
    showCustomCursor = (currentState != MenuState::GAME);
    window.setMouseCursorVisible(!showCustomCursor);
}

void Menu::updateLoadingSpinner() {
    if (isLoading) {
        spinnerRotation += 720.0f * loadingClock.getElapsedTime().asSeconds(); // 2 tours par seconde
        loadingSpinner.setRotation(spinnerRotation);
        
        if (loadingClock.getElapsedTime().asSeconds() >= 1.0f) {
            isLoading = false;
        }
    }
}

void Menu::updateWinScreenPositions() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    // Mise à jour du fond
    winBackground.setSize(sf::Vector2f(windowWidth, windowHeight));
    
    // Mise à jour du texte "PARTIE TERMINEE"
    winText.setCharacterSize(windowHeight * 0.1f);
    sf::FloatRect textBounds = winText.getLocalBounds();
    winText.setPosition(
        (windowWidth - textBounds.width) / 2,
        windowHeight * 0.2f
    );
    
    // Mise à jour du bouton "Retour aux salles"
    float buttonWidth = windowWidth * 0.25f;
    float buttonHeight = windowHeight * 0.08f;
    float centerX = (windowWidth - buttonWidth) / 2;
    
    backToRoomsButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    backToRoomsButton.setPosition(centerX, windowHeight * 0.5f);
    
    // Mise à jour du texte du bouton
    backToRoomsText.setCharacterSize(windowHeight * 0.04f);
    sf::FloatRect backBounds = backToRoomsText.getLocalBounds();
    backToRoomsText.setPosition(
        centerX + (buttonWidth - backBounds.width) / 2,
        windowHeight * 0.5f + (buttonHeight - backBounds.height) / 2
    );
}
