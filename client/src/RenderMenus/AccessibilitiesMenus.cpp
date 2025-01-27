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

std::string Menu::getKeyName(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::A: return "A";
        case sf::Keyboard::B: return "B";
        case sf::Keyboard::C: return "C";
        case sf::Keyboard::D: return "D";
        case sf::Keyboard::E: return "E";
        case sf::Keyboard::F: return "F";
        case sf::Keyboard::G: return "G";
        case sf::Keyboard::H: return "H";
        case sf::Keyboard::I: return "I";
        case sf::Keyboard::J: return "J";
        case sf::Keyboard::K: return "K";
        case sf::Keyboard::L: return "L";
        case sf::Keyboard::M: return "M";
        case sf::Keyboard::N: return "N";
        case sf::Keyboard::O: return "O";
        case sf::Keyboard::P: return "P";
        case sf::Keyboard::Q: return "Q";
        case sf::Keyboard::R: return "R";
        case sf::Keyboard::S: return "S";
        case sf::Keyboard::T: return "T";
        case sf::Keyboard::U: return "U";
        case sf::Keyboard::V: return "V";
        case sf::Keyboard::W: return "W";
        case sf::Keyboard::X: return "X";
        case sf::Keyboard::Y: return "Y";
        case sf::Keyboard::Z: return "Z";

        // Touches spéciales
        case sf::Keyboard::Space: return "Espace";
        case sf::Keyboard::Return: return "Entrée";
        case sf::Keyboard::BackSpace: return "Retour";
        case sf::Keyboard::Tab: return "Tab";
        case sf::Keyboard::Left: return "Gauche";
        case sf::Keyboard::Right: return "Droite";
        case sf::Keyboard::Up: return "Haut";
        case sf::Keyboard::Down: return "Bas";
        case sf::Keyboard::LShift: return "Shift G";
        case sf::Keyboard::RShift: return "Shift D";
        case sf::Keyboard::LControl: return "Ctrl G";
        case sf::Keyboard::RControl: return "Ctrl D";
        case sf::Keyboard::LAlt: return "Alt G";
        case sf::Keyboard::RAlt: return "Alt D";

        // Touches numériques
        case sf::Keyboard::Num0: return "0";
        case sf::Keyboard::Num1: return "1";
        case sf::Keyboard::Num2: return "2";
        case sf::Keyboard::Num3: return "3";
        case sf::Keyboard::Num4: return "4";
        case sf::Keyboard::Num5: return "5";
        case sf::Keyboard::Num6: return "6";
        case sf::Keyboard::Num7: return "7";
        case sf::Keyboard::Num8: return "8";
        case sf::Keyboard::Num9: return "9";

        default: return "?";
    }
}

void Menu::applyDaltonianColors() {
    if (isDaltonianMode) {
        // Couleurs daltonien
        sf::Color buttonColor(0, 170, 230);      // Bleu clair
        sf::Color textColor(255, 255, 0);        // Jaune
        sf::Color backgroundButton(50, 50, 150); // Bleu foncé
        
        // Ajout des éléments manquants
        loginButton.setFillColor(buttonColor);
        registerButton.setFillColor(buttonColor);
        loginText.setFillColor(textColor);
        //registerText.setFillColor(textColor);
        loginInput.setFillColor(backgroundButton);
        passwordLoginInput.setFillColor(backgroundButton);
        
        // Boutons de recherche et création
        //searchButton.setFillColor(buttonColor);
        searchText.setFillColor(textColor);
        createRoomText.setFillColor(textColor);
        
        // Textes des inputs
        nameInput.setFillColor(backgroundButton);
        passwordInput.setFillColor(backgroundButton);
        maxPlayersInput.setFillColor(backgroundButton);
        
        // Textes des labels
        nameLabel.setFillColor(textColor);
        passwordLabel.setFillColor(textColor);
        maxPlayersLabel.setFillColor(textColor);
        
        // Boutons principaux
        soloButton.setFillColor(buttonColor);
        multiButton.setFillColor(buttonColor);
        quitButton.setFillColor(buttonColor);
        settingsButton.setFillColor(buttonColor);
        accessibilityButton.setFillColor(buttonColor);
        daltonianButton.setFillColor(buttonColor);
        
        // Textes principaux
        soloText.setFillColor(textColor);
        multiText.setFillColor(textColor);
        quitText.setFillColor(textColor);
        settingsText.setFillColor(textColor);
        accessibilityText.setFillColor(textColor);
        daltonianText.setFillColor(textColor);
        settingsTitle.setFillColor(textColor);
        configTitle.setFillColor(textColor);
                // Éléments du menu des paramètres
        volumeSlider.setFillColor(backgroundButton);
        volumeSliderHandle.setFillColor(buttonColor);
        volumeLabel.setFillColor(textColor);
        
        // Éléments du lobby
        roomTitle.setFillColor(textColor);
        playersList.setFillColor(textColor);
        startText.setFillColor(textColor);
        backText.setFillColor(textColor);

        // Boutons du lobby
        lobbybackButton.setFillColor(buttonColor);
        lobbystartButton.setFillColor(buttonColor);
        
        // Éléments de la sélection de room
        backButton.setFillColor(buttonColor);
        refreshButton.setFillColor(buttonColor);
        createRoomButton.setFillColor(buttonColor);
        searchBar.setFillColor(backgroundButton);
        
        // Boutons des rooms
        for (auto& button : roomButtons) {
            button.setFillColor(backgroundButton);
        }
                            // Labels des touches
        for (auto& label : keyLabels) {
            label.setFillColor(textColor);
        }
        
        // Boutons "Rejoindre"
        for (auto& button : joinButtons) {
            button.setFillColor(buttonColor);
        }
        
        // Textes des rooms
        for (auto& text : roomTexts) {
            text.setFillColor(textColor);
        }
        
        // Textes des boutons "Rejoindre"
        for (auto& text : joinButtonTexts) {
            text.setFillColor(textColor);
        }

                for (auto& keyText : keyTexts) {
            keyText.setFillColor(textColor);
        }
        
        // Éléments de création de room
        nameInput.setFillColor(backgroundButton);
        passwordInput.setFillColor(backgroundButton);
        maxPlayersInput.setFillColor(backgroundButton);
        submitButton.setFillColor(buttonColor);
        
        // Textes de création de room
        nameText.setFillColor(textColor);
        nameLabel.setFillColor(textColor);
        passwordText.setFillColor(textColor);
        passwordLabel.setFillColor(textColor);
        maxPlayersText.setFillColor(textColor);
        maxPlayersLabel.setFillColor(textColor);
        submitText.setFillColor(textColor);
        
        // Menu pause
        resumeButton.setFillColor(buttonColor);
        mainMenuButton.setFillColor(buttonColor);
        resumeText.setFillColor(textColor);
        mainMenuText.setFillColor(textColor);

                // Boutons de difficulté
        difficultyEasyButton.setFillColor(buttonColor);
        difficultyMediumButton.setFillColor(buttonColor);
        difficultyHardButton.setFillColor(buttonColor);
        
        // Textes des boutons de difficulté
        difficultyEasyText.setFillColor(textColor);
        difficultyMediumText.setFillColor(textColor);
        difficultyHardText.setFillColor(textColor);
                // Titres du menu des paramètres
        settingsTitle.setFillColor(textColor);
        configTitle.setFillColor(textColor);

        // Ajout des couleurs pour le chat
        chatBackground.setFillColor(backgroundButton);
        chatBackground.setOutlineColor(buttonColor);
        
        for (auto& text : chatMessages) {
            text.setFillColor(textColor);
        }
        
        chatInput.setFillColor(backgroundButton);
        chatInput.setOutlineColor(buttonColor);
        chatInputText.setFillColor(textColor);
        
        // Couleurs du scoreboard
        scoreboardBackground.setFillColor(backgroundButton);
        scoreboardBackground.setOutlineColor(buttonColor);
        scoreboardTitle.setFillColor(textColor);
        
        for (auto& entry : scoreboardEntries) {
            if (entry.getFillColor() == sf::Color::White) {
                entry.setFillColor(textColor);
            }
        }

    } else {
        // Couleurs normales
        sf::Color defaultColor = isHighContrastMode ? sf::Color::White : sf::Color(120, 120, 120);
        sf::Color textColor = isHighContrastMode ? sf::Color::Black : sf::Color::White;
        sf::Color inputColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);
        
        // Ajout des éléments manquants
        loginButton.setFillColor(defaultColor);
        registerButton.setFillColor(defaultColor);
        loginText.setFillColor(textColor);
        //registerText.setFillColor(textColor);
        loginInput.setFillColor(inputColor);
        passwordLoginInput.setFillColor(inputColor);
        
        // Boutons de recherche et création
        //searchButton.setFillColor(defaultColor);
        searchText.setFillColor(textColor);
        createRoomText.setFillColor(textColor);
        
        // Textes des inputs
        nameInput.setFillColor(inputColor);
        passwordInput.setFillColor(inputColor);
        maxPlayersInput.setFillColor(inputColor);
        
        // Textes des labels
        nameLabel.setFillColor(textColor);
        passwordLabel.setFillColor(textColor);
        maxPlayersLabel.setFillColor(textColor);
        
        // Retour aux couleurs normales
        sf::Color roomColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);
        sf::Color joinColor = isHighContrastMode ? sf::Color::White : sf::Color(90, 90, 90);
        
        // Boutons principaux
        soloButton.setFillColor(defaultColor);
        multiButton.setFillColor(defaultColor);
        quitButton.setFillColor(defaultColor);
        settingsButton.setFillColor(defaultColor);
        accessibilityButton.setFillColor(defaultColor);
        daltonianButton.setFillColor(defaultColor);
        
        // Textes principaux
        soloText.setFillColor(textColor);
        multiText.setFillColor(textColor);
        quitText.setFillColor(textColor);
        settingsText.setFillColor(textColor);
        accessibilityText.setFillColor(textColor);
        daltonianText.setFillColor(textColor);
                // Textes du lobby
        roomTitle.setFillColor(textColor);
        playersList.setFillColor(textColor);
        startText.setFillColor(textColor);
        backText.setFillColor(textColor);



        difficultyEasyText.setFillColor(textColor);
        difficultyMediumText.setFillColor(textColor);
        difficultyHardText.setFillColor(textColor);

                // Boutons du lobby
        lobbybackButton.setFillColor(defaultColor);
        lobbystartButton.setFillColor(isHighContrastMode ? sf::Color::White : sf::Color(0, 150, 0));

                // Menu des paramètres
        settingsTitle.setFillColor(textColor);
        configTitle.setFillColor(textColor);
                // Éléments du menu des paramètres
        volumeSlider.setFillColor(sf::Color(70, 70, 70));
        volumeSliderHandle.setFillColor(sf::Color(150, 150, 150));
        volumeLabel.setFillColor(textColor);

        searchBar.setFillColor(sf::Color::White);
        searchText.setFillColor(sf::Color::Black);
        
        // Labels des touches
        for (auto& label : keyLabels) {
            label.setFillColor(textColor);
        }
        
        // Textes des touches
        for (auto& keyText : keyTexts) {
            keyText.setFillColor(textColor);
        }

        // Retour aux couleurs normales
        chatBackground.setFillColor(sf::Color(30, 30, 30, 200));
        chatBackground.setOutlineColor(sf::Color(100, 100, 100));
        
        for (auto& text : chatMessages) {
            text.setFillColor(textColor);
        }
        
        chatInput.setFillColor(defaultColor);
        chatInput.setOutlineColor(sf::Color(100, 100, 100));
        chatInputText.setFillColor(textColor);
        
        scoreboardBackground.setFillColor(sf::Color(30, 30, 30, 200));
        scoreboardBackground.setOutlineColor(sf::Color(100, 100, 100));
        scoreboardTitle.setFillColor(textColor);
    }
    centerAllButtonTexts();
}

void Menu::handleDaltonianEvents(const sf::Event& event) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (daltonianButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isDaltonianMode = !isDaltonianMode;
            updateDaltonianMode();
            applyDaltonianColors();
            
            // Forcer la mise à jour des éléments du menu
            updateRoomElements();
            updateAccessibilityStyles();
            centerAllButtonTexts();
            
            // Réinitialiser les couleurs des boutons si le mode est désactivé
            if (!isDaltonianMode) {
                resetRoomColors();
            }
        }
    }
}

void Menu::updateAccessibilityStyles() {
    float windowHeight = window.getSize().y;
    float baseTextSize = windowHeight * 0.03f;
    
    // Définition des couleurs au début de la fonction
    sf::Color defaultColor = isHighContrastMode ? sf::Color::White : sf::Color(120, 120, 120);
    sf::Color textColor = isHighContrastMode ? sf::Color::Black : sf::Color::White;
    sf::Color inputColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);

    if (isHighContrastMode) {
        // Mise à jour du bouton des paramètres
        settingsButton.setFillColor(sf::Color::White);
        settingsButton.setOutlineColor(sf::Color::Black);
        settingsButton.setOutlineThickness(2.0f);
        
        settingsText.setFont(robotoFont);
        settingsText.setFillColor(sf::Color::Black);
        settingsText.setCharacterSize(baseTextSize * 1.2f);
        
        // Pour les textes des rooms
        for (auto& text : roomTexts) {
            text.setFont(robotoFont);
            text.setFillColor(sf::Color::Black);
        }
        // Pour les boutons "Rejoindre"
        for (auto& text : joinButtonTexts) {
            text.setFont(robotoFont);
            text.setFillColor(sf::Color::Black);
        }
        
        // Pour les boutons des rooms
        for (auto& button : roomButtons) {
            button.setFillColor(sf::Color::White);
            button.setOutlineColor(sf::Color::Black);
            button.setOutlineThickness(2.0f);
        }
        
        // Pour les boutons "Rejoindre"
        for (auto& button : joinButtons) {
            button.setFillColor(sf::Color::White);
            button.setOutlineColor(sf::Color::Black);
            button.setOutlineThickness(2.0f);
        }

        // Menu paramètres
        volumeLabel.setFont(robotoFont);
        volumeLabel.setFillColor(sf::Color::Black);
        volumeLabel.setCharacterSize(volumeLabel.getCharacterSize() * 1.2f);

        volumeSlider.setFillColor(sf::Color::White);
        volumeSlider.setOutlineColor(sf::Color::Black);
        volumeSlider.setOutlineThickness(2.0f);

        volumeSliderHandle.setFillColor(sf::Color::Black);

        daltonianButton.setFillColor(sf::Color::White);
        daltonianButton.setOutlineColor(sf::Color::Black);
        daltonianButton.setOutlineThickness(2.0f);
        
        daltonianText.setFont(robotoFont);
        daltonianText.setFillColor(sf::Color::Black);
        daltonianText.setCharacterSize(daltonianText.getCharacterSize() * 1.2f);

        roomTitle.setFont(robotoFont);
        roomTitle.setFillColor(sf::Color::Black);
        roomTitle.setCharacterSize(roomTitle.getCharacterSize() * 1.2f);

        playersList.setFont(robotoFont);
        playersList.setFillColor(sf::Color::Black);
        playersList.setCharacterSize(playersList.getCharacterSize() * 1.2f);

        startText.setFont(robotoFont);
        startText.setFillColor(sf::Color::Black);
        startText.setCharacterSize(startText.getCharacterSize() * 1.2f);

        lobbybackButton.setFillColor(sf::Color::White);
        lobbybackButton.setOutlineColor(sf::Color::Black);
        lobbybackButton.setOutlineThickness(2.0f);

        lobbystartButton.setFillColor(sf::Color::White);
        lobbystartButton.setOutlineColor(sf::Color::Black);
        lobbystartButton.setOutlineThickness(2.0f);

        sf::Color textColor = isHighContrastMode ? sf::Color::White : sf::Color::White;
        sf::Color bgColor = isHighContrastMode ? sf::Color::Black : sf::Color(50, 50, 50);
        sf::Color inputBgColor = isHighContrastMode ? sf::Color::White : sf::Color(30, 30, 30);
        sf::Color buttonColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);

            // Chat
        chatInput.setFillColor(inputBgColor);
        chatInput.setOutlineColor(isHighContrastMode ? sf::Color::White : sf::Color(100, 100, 100));
        //chatBackground.setFillColor(bgColor);
        //chatBackground.setOutlineColor(isHighContrastMode ? sf::Color::White : sf::Color(100, 100, 100));
        
    for (auto& text : chatMessages) {
        text.setFillColor(textColor);
    }
    
    chatInputText.setFillColor(isHighContrastMode ? sf::Color::Black : sf::Color::White);

        // Ajout de la gestion des textes de configuration des touches
        std::vector<std::pair<std::string, sf::Keyboard::Key*>> bindings = {
            {"Haut", &keyBindings.moveUp},
            {"Bas", &keyBindings.moveDown},
            {"Gauche", &keyBindings.moveLeft},
            {"Droite", &keyBindings.moveRight},
            {"Tirer", &keyBindings.shoot}
        };

        // Titre des paramètres
        settingsTitle.setFont(robotoFont);
        settingsTitle.setFillColor(sf::Color::Black);
        settingsTitle.setCharacterSize(settingsTitle.getCharacterSize() * 1.2f);

        // Titre de la configuration des touches
        configTitle.setFont(robotoFont);
        configTitle.setFillColor(sf::Color::Black);
        configTitle.setCharacterSize(configTitle.getCharacterSize() * 1.2f);

        // Labels des touches
        for (const auto& binding : bindings) {
            sf::Text label;
            label.setFont(robotoFont);
            label.setFillColor(sf::Color::Black);
            label.setCharacterSize(window.getSize().y * 0.03f);
            keyLabels.push_back(label);
        }

        // Textes des touches
        for (const auto& binding : bindings) {
            sf::Text keyText;
            keyText.setFont(robotoFont);
            keyText.setFillColor(sf::Color::Black);
            keyText.setCharacterSize(window.getSize().y * 0.03f);
            keyTexts.push_back(keyText);
        }

        // Ajouter le texte des boutons
        lobbybackText.setFont(robotoFont);
        lobbybackText.setFillColor(sf::Color::Black);
        lobbybackText.setCharacterSize(lobbybackText.getCharacterSize() * 1.2f);

        startText.setFont(robotoFont);
        startText.setFillColor(sf::Color::Black);
        startText.setCharacterSize(startText.getCharacterSize() * 1.2f);

                // Boutons de difficulté
        difficultyEasyButton.setFillColor(sf::Color::White);
        difficultyEasyButton.setOutlineColor(sf::Color::Black);
        difficultyEasyButton.setOutlineThickness(2.0f);
        
        difficultyMediumButton.setFillColor(sf::Color::White);
        difficultyMediumButton.setOutlineColor(sf::Color::Black);
        difficultyMediumButton.setOutlineThickness(2.0f);
        
        difficultyHardButton.setFillColor(sf::Color::White);
        difficultyHardButton.setOutlineColor(sf::Color::Black);
        difficultyHardButton.setOutlineThickness(2.0f);
        
        // Textes des boutons de difficulté
        difficultyEasyText.setFont(robotoFont);
        difficultyEasyText.setFillColor(sf::Color::Black);
        difficultyEasyText.setCharacterSize(difficultyEasyText.getCharacterSize() * 1.2f);
        
        difficultyMediumText.setFont(robotoFont);
        difficultyMediumText.setFillColor(sf::Color::Black);
        difficultyMediumText.setCharacterSize(difficultyMediumText.getCharacterSize() * 1.2f);
        
        difficultyHardText.setFont(robotoFont);
        difficultyHardText.setFillColor(sf::Color::Black);
        difficultyHardText.setCharacterSize(difficultyHardText.getCharacterSize() * 1.2f);

        // Texte du bouton lancer la partie
        startText.setFont(robotoFont);
        startText.setFillColor(sf::Color::Black);
        startText.setCharacterSize(startText.getCharacterSize() * 1.2f);

        // Bouton de retour
        backButton.setFillColor(sf::Color::White);
        backButton.setOutlineColor(sf::Color::Black);
        backButton.setOutlineThickness(2.0f);

        // Bouton rafraîchir
        refreshButton.setFillColor(sf::Color::White);
        refreshButton.setOutlineColor(sf::Color::Black);
        refreshButton.setOutlineThickness(2.0f);
        refreshText.setFont(robotoFont);
        refreshText.setFillColor(sf::Color::Black);
        refreshText.setCharacterSize(refreshText.getCharacterSize() * 1.2f);

        // text lobby
        lobbybackText.setFont(robotoFont);
        lobbybackText.setFillColor(sf::Color::White);
        lobbybackText.setCharacterSize(lobbybackText.getCharacterSize() * 1.2f);

        backText.setFont(robotoFont);
        backText.setFillColor(sf::Color::Black);
        backText.setCharacterSize(backText.getCharacterSize() * 1.2f);

                loginButton.setFillColor(sf::Color::White);
        loginButton.setOutlineColor(sf::Color::Black);
        loginButton.setOutlineThickness(2.0f);
        
        registerButton.setFillColor(sf::Color::White);
        registerButton.setOutlineColor(sf::Color::Black);
        registerButton.setOutlineThickness(2.0f);
        
        //searchButton.setFillColor(sf::Color::White);
        //searchButton.setOutlineColor(sf::Color::Black);
        //searchButton.setOutlineThickness(2.0f);
        
        // Textes
        loginText.setFont(robotoFont);
        loginText.setFillColor(sf::Color::Black);
       // registerText.setFont(robotoFont);
       // registerText.setFillColor(sf::Color::Black);
        searchText.setFont(robotoFont);
        searchText.setFillColor(sf::Color::Black);

    } else {
        // Retour à la normale pour le bouton des paramètres
        settingsButton.setFillColor(isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(70, 70, 70));
        settingsButton.setOutlineThickness(0.0f);
        
        settingsText.setFont(font);
        settingsText.setFillColor(isDaltonianMode ? sf::Color::Yellow : sf::Color::White);
        settingsText.setCharacterSize(baseTextSize);
        
        // Pour les textes des rooms
        for (auto& text : roomTexts) {
            text.setFont(font);
            text.setFillColor(textColor);
            text.setCharacterSize(window.getSize().y * 0.03f);
        }
        
        // Pour les boutons "Rejoindre"
        for (auto& text : joinButtonTexts) {
            text.setFont(font);
            text.setFillColor(textColor);
            text.setCharacterSize(window.getSize().y * 0.03f);
        }
        
        // Pour les boutons des rooms
        for (auto& button : roomButtons) {
            button.setFillColor(defaultColor);
            button.setOutlineThickness(0.0f);
        }
        
        // Pour les boutons "Rejoindre"
        for (auto& button : joinButtons) {
            button.setFillColor(defaultColor);
            button.setOutlineThickness(0.0f);
        }

        // Menu paramètres
        volumeLabel.setFont(font);
        volumeLabel.setFillColor(textColor);
        volumeLabel.setCharacterSize(window.getSize().y * 0.03f);

        volumeSlider.setFillColor(sf::Color(70, 70, 70));
        volumeSlider.setOutlineThickness(0.0f);

        volumeSliderHandle.setFillColor(sf::Color(150, 150, 150));

        daltonianButton.setFillColor(sf::Color(70, 70, 70));
        daltonianButton.setOutlineThickness(0.0f);
        
        daltonianText.setFont(font);
        daltonianText.setFillColor(textColor);
        daltonianText.setCharacterSize(window.getSize().y * 0.03f);

         // Éléments du lobby
        roomTitle.setFont(font);
        roomTitle.setFillColor(textColor);
        roomTitle.setCharacterSize(window.getSize().y * 0.04f);

        playersList.setFont(font);
        playersList.setFillColor(textColor);
        playersList.setCharacterSize(window.getSize().y * 0.03f);

        startText.setFont(font);
        startText.setFillColor(textColor);
        startText.setCharacterSize(window.getSize().y * 0.03f);

        lobbybackButton.setFillColor(sf::Color(100, 100, 100));
        lobbybackButton.setOutlineThickness(0.0f);

        lobbystartButton.setFillColor(sf::Color(0, 150, 0));
        lobbystartButton.setOutlineThickness(0.0f);

        // Version normale
        settingsTitle.setFont(font);
        settingsTitle.setFillColor(textColor);
        settingsTitle.setCharacterSize(settingsTitle.getCharacterSize() / 1.2f);

        configTitle.setFont(font);
        configTitle.setFillColor(textColor);
        configTitle.setCharacterSize(configTitle.getCharacterSize() / 1.2f);

        for (auto& label : keyLabels) {
            label.setFont(font);
            label.setFillColor(textColor);
            label.setCharacterSize(window.getSize().y * 0.03f);
        }

        for (auto& keyText : keyTexts) {
            keyText.setFont(font);
            keyText.setFillColor(textColor);
            keyText.setCharacterSize(window.getSize().y * 0.03f);
        }

        // Retour à la normale pour le texte des boutons
        lobbybackText.setFont(font);
        lobbybackText.setFillColor(textColor);
        lobbybackText.setCharacterSize(window.getSize().y * 0.03f);

        startText.setFont(font);
        startText.setFillColor(textColor);
        startText.setCharacterSize(window.getSize().y * 0.03f);

                // Retour aux styles normaux
        difficultyEasyButton.setFillColor(sf::Color(100, 100, 100));
        difficultyMediumButton.setFillColor(sf::Color(100, 100, 100));
        difficultyHardButton.setFillColor(sf::Color(100, 100, 100));
        
        difficultyEasyText.setFont(font);
        difficultyEasyText.setFillColor(textColor);
        difficultyEasyText.setCharacterSize(window.getSize().y * 0.03f);
        
        difficultyMediumText.setFont(font);
        difficultyMediumText.setFillColor(textColor);
        difficultyMediumText.setCharacterSize(window.getSize().y * 0.03f);
        
        difficultyHardText.setFont(font);
        difficultyHardText.setFillColor(textColor);
        difficultyHardText.setCharacterSize(window.getSize().y * 0.03f);

        // text lobby
        lobbybackText.setFont(font);
        lobbybackText.setFillColor(textColor);
        lobbybackText.setCharacterSize(lobbybackText.getCharacterSize() * 1.2f);

        // bouton lancer la partie
        startText.setFont(font);
        startText.setFillColor(textColor);
        startText.setCharacterSize(startText.getCharacterSize() * 1.2f);

        backText.setFont(font);
        backText.setFillColor(textColor);
        backText.setCharacterSize(window.getSize().y * 0.03f);

              loginButton.setFillColor(sf::Color(100, 100, 100));
        loginButton.setOutlineThickness(0.0f);
        
        registerButton.setFillColor(sf::Color(100, 100, 100));
        registerButton.setOutlineThickness(0.0f);
        
        //searchButton.setFillColor(sf::Color(100, 100, 100));
        //searchButton.setOutlineThickness(0.0f);
        
        // Textes
        loginText.setFont(font);
        loginText.setFillColor(sf::Color::White);
        //registerText.setFont(font);
        //registerText.setFillColor(sf::Color::White);
        searchText.setFont(font);
        searchText.setFillColor(sf::Color::White);
        
        // Bouton rafraîchir
        refreshButton.setFillColor(sf::Color(100, 100, 100));
        refreshButton.setOutlineThickness(0.0f);
        refreshText.setFont(font);
        refreshText.setFillColor(textColor);
        refreshText.setCharacterSize(window.getSize().y * 0.03f);
    }
}

void Menu::updateRoomElements() {
    sf::Color buttonColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);

    // Pour les boutons de room
    for (auto& button : roomButtons) {
        button.setFillColor(buttonColor);
        if (isHighContrastMode) {
            button.setOutlineColor(sf::Color::Black);
            button.setOutlineThickness(2.0f);
        }
    }

    // Pour les boutons "Rejoindre"
    for (auto& button : joinButtons) {
        button.setFillColor(buttonColor);
        if (isHighContrastMode) {
            button.setOutlineColor(sf::Color::Black);
            button.setOutlineThickness(2.0f);
        }
    }

    // Bouton retour et rafraîchir
    backButton.setFillColor(buttonColor);
    refreshButton.setFillColor(buttonColor);
    createRoomButton.setFillColor(buttonColor);

    if (isHighContrastMode) {
        backButton.setOutlineColor(sf::Color::Black);
        backButton.setOutlineThickness(2.0f);
        refreshButton.setOutlineColor(sf::Color::Black);
        refreshButton.setOutlineThickness(2.0f);
        createRoomButton.setOutlineColor(sf::Color::Black);
        createRoomButton.setOutlineThickness(2.0f);
    }
}

void Menu::updateDaltonianMode() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    float daltonianButtonWidth = windowWidth * 0.20f;
    float daltonianButtonHeight = windowHeight * 0.05f;
    
    daltonianButton.setSize(sf::Vector2f(daltonianButtonWidth, daltonianButtonHeight));
    daltonianButton.setPosition(windowWidth * 0.05f, windowHeight * 0.15f);

    if (!isDaltonianMode && !isHighContrastMode) {
        daltonianButton.setFillColor(sf::Color(100, 100, 100));
        daltonianButton.setOutlineThickness(0.0f);
        daltonianText.setFillColor(sf::Color::White);
        daltonianText.setFont(font);
    }
    
    if (isHighContrastMode) {
        daltonianButton.setFillColor(sf::Color::White);
        daltonianButton.setOutlineColor(sf::Color::Black);
        daltonianButton.setOutlineThickness(2.0f);
        daltonianText.setFillColor(sf::Color::Black);
        daltonianText.setFont(robotoFont);
    } else {
        daltonianButton.setFillColor(isDaltonianMode ? sf::Color(0, 170, 230) : sf::Color(100, 100, 100));
        daltonianButton.setOutlineThickness(0.0f);
        daltonianText.setFillColor(sf::Color::White);
        daltonianText.setFont(font);
    }
    
    daltonianText.setString("Mode Daltonien");
    daltonianText.setCharacterSize(windowHeight * 0.03f);
    centerTextInButton(daltonianText, daltonianButton);

    // Mise à jour des éléments du menu des paramètres
    if (isHighContrastMode || isDaltonianMode) {
        // Titres
        settingsTitle.setFont(robotoFont);
        settingsTitle.setFillColor(sf::Color::Black);
        configTitle.setFont(robotoFont);
        configTitle.setFillColor(sf::Color::Black);
        
        // Labels des touches
        for (auto& label : keyLabels) {
            label.setFont(robotoFont);
            label.setFillColor(sf::Color::Black);
        }
        
        // Textes des touches
        for (auto& keyText : keyTexts) {
            keyText.setFont(robotoFont);
            keyText.setFillColor(sf::Color::Black);
        }
    } else {
        // Retour aux styles normaux
        settingsTitle.setFont(font);
        settingsTitle.setFillColor(sf::Color::White);
        configTitle.setFont(font);
        configTitle.setFillColor(sf::Color::White);
        
        // Labels des touches
        for (auto& label : keyLabels) {
            label.setFont(font);
            label.setFillColor(sf::Color::White);
        }
        
        // Textes des touches
        for (auto& keyText : keyTexts) {
            keyText.setFont(font);
            keyText.setFillColor(sf::Color::White);
        }
    }
    centerAllButtonTexts();
}

void Menu::centerAllButtonTexts() {
    // Boutons principaux
    centerTextInButton(soloText, soloButton);
    centerTextInButton(multiText, multiButton);
    centerTextInButton(quitText, quitButton);
    centerTextInButton(settingsText, settingsButton);
    centerTextInButton(accessibilityText, accessibilityButton);
    centerTextInButton(daltonianText, daltonianButton);
    centerTextInButton(refreshText, refreshButton);
    centerTextInButton(backText, backButton);
    centerTextInButton(createRoomText, createRoomButton);
   // centerTextInButton(startText);
    
    // Boutons de difficulté
    centerTextInButton(difficultyEasyText, difficultyEasyButton);
    centerTextInButton(difficultyMediumText, difficultyMediumButton);
    centerTextInButton(difficultyHardText, difficultyHardButton);
    
    // Boutons de login/register
    //centerTextInButton(loginText, loginButton);
    //centerTextInButton(registerText, registerButton);
    
    // Boutons des rooms
    for (size_t i = 0; i < joinButtons.size(); ++i) {
        centerTextInButton(joinButtonTexts[i], joinButtons[i]);
    }
}
