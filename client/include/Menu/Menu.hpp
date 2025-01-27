/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Menu
*/

#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "MenuState.hpp"
#include "../ECS/TextureManager.hpp"
#include "../Network/NetworkManager.hpp"
#include "../ECS/SoundManager.hpp"
#include "../Parser/jsonParser.hpp"
#include <functional>
#include <map>

// Déclaration anticipée
class World;

struct Room {
    std::string id;
    std::string name;
    std::string password;
    std::string state;
    std::string maxPlayers;
    std::string currentPlayers;

    Room() = default;
    Room(const Room&) = default;
    Room& operator=(const Room&) = default;
    Room(Room&&) = default;
    Room& operator=(Room&&) = default;
};

class Menu {
    private:
        sf::Font font;
        sf::Font robotoFont;
        sf::RenderWindow& window;
        MenuState currentState;
        MenuState previousState;
       
        std::vector<Room> rooms;
        void updateRoomElements();
        
        // Boutons du menu principal
        sf::RectangleShape soloButton;
        sf::RectangleShape multiButton;
        sf::Text soloText;
        sf::Text multiText;
        
        // Éléments pour la liste des rooms
        std::vector<sf::RectangleShape> roomButtons;
        std::vector<sf::Text> roomTexts;
        sf::RectangleShape backButton;
        sf::Text backText;
        
        // Éléments pour le menu pause
        sf::RectangleShape resumeButton;
        sf::RectangleShape mainMenuButton;
        sf::Text resumeText;
        sf::Text mainMenuText;
        sf::RectangleShape pauseBackground;
        
        // Curseur personnalisé
        sf::Texture cursorTexture;
        sf::Sprite cursorSprite;
        bool showCustomCursor;
        
        sf::RectangleShape quitButton;
        sf::Text quitText;

        using StateChangeCallback = std::function<void(MenuState)>;
        StateChangeCallback stateChangeCallback;

        NetworkManager networkManager;

        sf::RectangleShape refreshButton;
        sf::Text refreshText;
        sf::RectangleShape searchBar;
        sf::Text searchText;
        std::string searchString;
        bool isSearchBarFocused;

        sf::Clock loadingClock;
        bool isLoading;
        sf::CircleShape loadingSpinner;
        float spinnerRotation;

        // Éléments pour le game over
        sf::RectangleShape gameOverBackground;
        sf::Text gameOverText;
        sf::RectangleShape restartButton;
        sf::RectangleShape mainMenuFromGameOverButton;
        sf::Text restartText;
        sf::Text mainMenuFromGameOverText;
        // Éléments pour la création de room
        sf::RectangleShape createRoomButton;
        sf::Text createRoomText;
        
        // Formulaire de création
        sf::RectangleShape nameInput;
        sf::Text nameText;
        sf::Text nameLabel;
        std::string nameString;
        
        sf::RectangleShape passwordInput;
        sf::Text passwordText;
        sf::Text passwordLabel;
        std::string passwordString;
        
        sf::RectangleShape maxPlayersInput;
        sf::Text maxPlayersText;
        sf::Text maxPlayersLabel;
        std::string maxPlayersString;
        
        sf::RectangleShape submitButton;
        sf::Text submitText;
        
        bool isNameFocused;
        bool isPasswordFocused;
        bool isMaxPlayersFocused;


        // Éléments pour la liste des rooms
        std::vector<sf::RectangleShape> joinButtons;
        std::vector<sf::Text> joinButtonTexts;



        // Éléments pour le prompt de mot de passe
        bool isPasswordPromptVisible;
        sf::RectangleShape passwordPromptInput;
        sf::Text passwordPromptText;
        sf::Text passwordPromptLabel;
        std::string passwordPromptString;
        sf::RectangleShape passwordPromptSubmit;
        sf::Text passwordPromptSubmitText;
        std::string currentRoomId;
        sf::RectangleShape passwordPromptBack;
        sf::Text passwordPromptBackText;

        // Éléments pour le lobby
        sf::RectangleShape lobbybackButton;
        sf::RectangleShape lobbystartButton;
        sf::Text lobbybackText;
        std::vector<std::string> lobbyPlayers;
        void updateLobbyPlayers(const std::string& playerData);

        sf::RectangleShape accessibilityButton;
        sf::Text accessibilityText;
        bool isHighContrastMode;
        float textScaleFactor;

        void centerTextInButton(sf::Text& text, const sf::RectangleShape& button);
        void updateRoomListPositions();
        void updatePauseMenuPositions();
        void updateGameOverPositions();


        // World
        World* world;


        // Joueurs en jeu
        std::map<int, float> playerShootCooldowns;

        // Après la déclaration des autres boutons
        sf::RectangleShape settingsButton;
        sf::Text settingsText;
        bool isSettingsMenuOpen;
        sf::RectangleShape scoreboardBackground;
        sf::Text scoreboardTitle;
        std::vector<sf::Text> scoreboardEntries;
        bool isScoreboardVisible;
        void createScoreboard();
        void updateScoreboard();

        void updateScoreboardEntries(const std::vector<std::pair<std::string, int>>& scores);


        // Structure pour stocker les touches
        struct KeyBindings {
            sf::Keyboard::Key moveUp;
            sf::Keyboard::Key moveDown;
            sf::Keyboard::Key moveLeft;
            sf::Keyboard::Key moveRight;
            sf::Keyboard::Key shoot;
        };
        KeyBindings keyBindings;

        // Variables pour la modification des touches
        bool isWaitingForKey;
        std::string currentKeyBinding;

        void renderSettingsMenu();
        void handleSettingsEvents(const sf::Event& event);
        std::string getKeyName(sf::Keyboard::Key key);

        sf::RectangleShape volumeSlider;
        sf::RectangleShape volumeSliderHandle;
        sf::Text volumeLabel;
        bool isDraggingVolume;
        float currentVolume;  // 0.0f à 1.0f

        bool isDaltonianMode;
        sf::RectangleShape daltonianButton;
        sf::Text daltonianText;

        void updateVolumeSliderPositions();

        // Textes du lobby
        sf::Text roomTitle;
        sf::Text playersList;
        sf::Text startText;

        void updateAccessibilityStyles();

        void updateDaltonianMode();
        void applyDaltonianColors();

        void handleDaltonianEvents(const sf::Event& event);

        // Éléments pour le menu des paramètres
        sf::Text settingsTitle;
        sf::Text configTitle;
        std::vector<sf::Text> keyLabels;
        std::vector<sf::Text> keyTexts;

        enum class DaltonianType {
            NONE,
            PROTANOPIA,    // Difficulté avec le rouge
            DEUTERANOPIA,  // Difficulté avec le vert
            TRITANOPIA     // Difficulté avec le bleu
        };

        DaltonianType currentDaltonianType;

        sf::RectangleShape difficultyEasyButton;
        sf::RectangleShape difficultyMediumButton;
        sf::RectangleShape difficultyHardButton;
        sf::Text difficultyEasyText;
        sf::Text difficultyMediumText;
        sf::Text difficultyHardText;
        std::string currentDifficulty;

        // Après la déclaration des boutons de difficulté
        sf::RectangleShape* activeDifficultyButton;

        // Éléments pour l'écran de victoire
        sf::RectangleShape winBackground;
        sf::Text winText;
        sf::RectangleShape mainMenuFromWinButton;
        sf::Text mainMenuFromWinText;

        sf::RectangleShape loginInput;
        sf::RectangleShape passwordLoginInput;
        sf::RectangleShape loginButton;
        sf::RectangleShape registerButton;
        sf::Text registerButtonText;
        sf::Text loginText;
        sf::Text passwordLoginText;
        sf::Text loginButtonText;
        sf::Text loginLabel;
        sf::Text passwordLoginLabel;
        std::string loginString;
        std::string passwordLoginString;
        bool isLoginFocused;
        bool isPasswordLoginFocused;

        sf::Text errorText;
        std::string errorMessage;
        float errorDisplayTime;
        bool showError;

        bool showLoginError = false;
        std::string loginErrorMessage = "";

        // Win screen elements
        sf::RectangleShape backToRoomsButton;
        sf::Text backToRoomsText;
        void updateWinScreenPositions();
        
        void createWinMenu();
        void handleWinEvents(const sf::Event& event);
        void renderWinScreen();

        // Éléments pour le chat
        sf::RectangleShape chatBox;
        sf::RectangleShape chatBackground;
        sf::RectangleShape chatInput;
        sf::Text chatInputText;
        std::string chatInputString;
        bool isChatFocused;
        std::vector<sf::Text> chatMessages;
        std::deque<std::string> messageHistory;
        static const size_t MAX_CHAT_MESSAGES = 10;

        // Chat methods
        void handleChatEvents(const sf::Event& event);
        void addChatMessage(const std::string& message, bool isPrivate = false);
        void updateChatMessages();
        void createChat();
        void renderChat();
        
        // Pour le bouton du chat
        sf::RectangleShape chatToggleButton;
        sf::Text chatToggleText;
        bool isChatVisible;

        
        std::atomic<bool> isRefreshing{false};

        void loadFonts() {
            static bool fontsLoaded = false;
            if (!fontsLoaded) {
                if (!font.loadFromFile("client/font/Vipnagorgialla.otf")) {
                    throw std::runtime_error("Impossible de charger la police Vipnagorgialla");
                }
                if (!robotoFont.loadFromFile("client/font/Roboto-Medium.ttf")) {
                    throw std::runtime_error("Impossible de charger la police Roboto");
                }
                fontsLoaded = true;
            }
        }

        void handleReceiveMessageMainMenu();

        void resetRoomColors() {
            sf::Color defaultColor = isHighContrastMode ? sf::Color::White : sf::Color(70, 70, 70);
            
            for (auto& button : roomButtons) {
                button.setFillColor(defaultColor);
            }
            
            for (auto& button : joinButtons) {
                button.setFillColor(defaultColor);
            }
        }

        void centerAllButtonTexts();

        void resizeMainMenu();

        void resizeCreateRoomMenu();

        // Bouton de matchmaking
        sf::RectangleShape matchmakingButton;
        sf::Text matchmakingText;

    public:
        static StateChangeCallback globalStateChangeCallback;
        
        Menu(sf::RenderWindow& window, World* gameWorld, std::string ip, std::string port);
        void handleEvent(const sf::Event& event);
        void render();
        void initializeRooms();
        void createButtons();
        void configureMainButtons();
        void configureButtonText();
        void configureAdditionalButtons();
        void configureInputFields();
        void configureAccessibilitySettings();  
        void handleRoomJoinEvent(const sf::Event& event, sf::RenderWindow& window);
        void renderPasswordPrompt();

        void renderMainMenu();
        void renderRoomSelect();
        void renderRoomLobby();
        void createPauseMenu();
        void renderPauseMenu();
        void setPaused(bool paused);
        void handleResize();
        void updateCursor();
        void updateRooms(const std::string& roomData);
        void updateLoadingSpinner();
        void updateVolumeHandlePosition();
        void setState(MenuState state);
        void handleTextInput(std::string& text, sf::Uint32 unicode);

        MenuState getState() const { 
            return currentState;
        }

        void setStateChangeCallback(StateChangeCallback callback) {
            stateChangeCallback = callback;
        }

        static void setGlobalState(MenuState newState) {
            if (globalStateChangeCallback) {
                globalStateChangeCallback(newState);
            }
        }

        MenuState getCurrentState() const {
            return currentState;
        }

        void createGameOverMenu();
        void renderGameOver();
        void createRoomForm();
        void renderRoomCreate();

        MenuState getPreviousState() const { return previousState; }

        void handleRoomLobbyEvents(const sf::Event& event);
        void handleRoomSelectEvents(const sf::Event& event);
        void handlePauseMenuEvents(const sf::Event& event);
        void handleGameOverEvents(const sf::Event& event);
        void handleRoomCreateEvents(const sf::Event& event);
        void handleMainMenuEvents(const sf::Event& event);
        void handlePasswordEvents(const sf::Event& event);
        void handleGameEvents();
        void handleInGameKeyboardEvents();
        void handleChangeSpritePlayer();

        KeyBindings& getKeyBindings() { return keyBindings; }

        void handleLoginEvents(const sf::Event& event);
};

