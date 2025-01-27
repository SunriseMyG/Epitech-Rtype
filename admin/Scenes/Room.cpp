#include "Room.hpp"

RoomScene::RoomScene(SceneManager& sceneManager, FontLoader& fontLoader) 
    : Scene(sceneManager, fontLoader),
        chat_(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.getWindow()),
        refreshButton(sf::Vector2f(110, 110), sf::Vector2f(200, 50), sf::Color(128, 128, 128), sceneManager.i18n_.get("refresh"),
            fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sf::Color::White),
        returnButton(sf::Vector2f(110, 170), sf::Vector2f(200, 50), sf::Color(128, 128, 128), sceneManager.i18n_.get("return"),
            fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sf::Color::White)
        {
    
    roomInfoBox.setSize(sf::Vector2f(400, 250));
    roomInfoBox.setFillColor(sf::Color(50, 50, 50, 200));
    roomInfoBox.setOutlineThickness(2);
    roomInfoBox.setOutlineColor(sf::Color::White);
    roomInfoBox.setPosition(sceneManager.getWindow().getSize().x / 2 - roomInfoBox.getSize().x / 2, refreshButton.getPosition().y + refreshButton.getSize().y + 20);

    roomNameText = new CreateText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), "Room Name: " + roomName_, 24, sf::Color::White, sf::Vector2f(roomInfoBox.getPosition().x + 10, roomInfoBox.getPosition().y + 10));
    roomPasswordText = new CreateText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), "Room Password: " + roomPassword_, 24, sf::Color::White, sf::Vector2f(roomInfoBox.getPosition().x + 10, roomInfoBox.getPosition().y + 40));
    roomStateText = new CreateText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), "Room State: " + roomState_, 24, sf::Color::White, sf::Vector2f(roomInfoBox.getPosition().x + 10, roomInfoBox.getPosition().y + 70));
    maxPlayersText = new CreateText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), "Max Players: " + std::to_string(maxPlayers_), 24, sf::Color::White, sf::Vector2f(roomInfoBox.getPosition().x + 10, roomInfoBox.getPosition().y + 100));
    currentPlayersText = new CreateText(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), "Current Players: " + std::to_string(currentPlayers_), 24, sf::Color::White, sf::Vector2f(roomInfoBox.getPosition().x + 10, roomInfoBox.getPosition().y + 130));
}

void RoomScene::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {

    for (auto &statCard : playerStats) {
        // a finir faut get l'id des joueurs sur la card qu'on clic
        statCard.handleEventKick(window, event, [&]() {
            sceneManager.getNetwork().sendKickPlayer(std::to_string(statCard.getPlayerId()));
        });
        statCard.handleEventBan(window, event, [&]() {
            sceneManager.getNetwork().sendBanPlayer(std::to_string(statCard.getPlayerId()));
        });
    }


    refreshButton.handleEvent(window, event, [&]() {
        if (refreshCooldown.getElapsedTime().asSeconds() >= REFRESH_COOLDOWN_SECONDS) {
            std::cout << "Refreshing room info" << std::endl;
            // sceneManager.getNetwork().sendGetRooms();
            sceneManager.getNetwork().sendGetPlayerStatInRoom(roomId_);
            // SceneManager
            refreshCooldown.restart();
        }
    });

    returnButton.handleEvent(window, event, [&]() {
        sceneManager.switchTo("global");
    });

    chat_.handleEvent(event, sceneManager.getWindow(), sceneManager.getNetwork());
}

void RoomScene::draw(sf::RenderWindow& window) {
    if (!loaded) {
        loader();
    }

    for (auto &stat : playerStats) {
        stat.draw(window);
    }

    window.draw(roomInfoBox);
    roomNameText->draw(window);
    roomPasswordText->draw(window);
    roomStateText->draw(window);
    maxPlayersText->draw(window);
    currentPlayersText->draw(window);
    refreshButton.draw(window);
    returnButton.draw(window);
    chat_.draw(window);
}

void RoomScene::setRoomInfo(const std::string &roomId, const std::string &roomName, const std::string &roomPassword, const std::string &roomState, const int maxPlayers, const int currentPlayers) {
    roomId_ = roomId;
    roomName_ = roomName;
    roomPassword_ = roomPassword;
    roomState_ = roomState;
    maxPlayers_ = maxPlayers;
    currentPlayers_ = currentPlayers;

    roomNameText->setText("Room Name: " + roomName_);
    roomPasswordText->setText("Room Password: " + roomPassword_);
    roomStateText->setText("Room State: " + roomState_);
    maxPlayersText->setText("Max Players: " + std::to_string(maxPlayers_));
    currentPlayersText->setText("Current Players: " + std::to_string(currentPlayers_));

    std::cout << "Room info: " << roomId_ << " " << roomName_ << " " << roomPassword_ << " " << roomState_ << " " << maxPlayers_ << " " << currentPlayers_ << std::endl;
}

void RoomScene::loader() {
    sceneManager.getNetwork().setPlayerStatInRoomCallback([&](const std::vector<Statistic> &stats) {
        for (const auto& stat : stats) {
            std::cout << "Player ID: " << stat.player_id << std::endl;
            std::cout << "Number of Deaths: " << stat.nbrDeaths << std::endl;
            std::cout << "Number of Games: " << stat.nbrGames << std::endl;
            std::cout << "Number of Kills: " << stat.nbrKills << std::endl;
            std::cout << "Number of Losses: " << stat.nbrLosses << std::endl;
            std::cout << "Number of Wins: " << stat.nbrWins << std::endl;
            std::cout << "Score: " << stat.score << std::endl;
        }
        playerStats.clear();
        const float CARD_WIDTH = 400;
        const float CARD_HEIGHT = 300;
        const float CARD_SPACING = 20;
        const float LEFT_PADDING = roomInfoBox.getPosition().x;
        const float TOP_PADDING = (roomInfoBox.getPosition().y * 2) + roomInfoBox.getSize().y + 20;
        // const float TOP_PADDING = 1500;
        sf::Vector2u windowSize = sceneManager.getWindow().getSize();
        float maxVisibleHeight = windowSize.y - TOP_PADDING;
        for (size_t i = 0; i < stats.size(); i++) {
            const auto& stat = stats[i];
            playerStats.push_back(CreateStatCard(
                sf::Vector2f(LEFT_PADDING + (i * (CARD_WIDTH + CARD_SPACING)), TOP_PADDING),
                sf::Vector2f(CARD_WIDTH, CARD_HEIGHT),
                sf::Color::White,
                fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"),
                sceneManager.i18n_.get("kick"),
                sceneManager.i18n_.get("ban"),
                "Id: " + stat.player_id,
                "Deaths: " + std::to_string(stat.nbrDeaths),
                "Games: " + std::to_string(stat.nbrGames),
                "Kills: " + std::to_string(stat.nbrKills),
                "Losses: " + std::to_string(stat.nbrLosses),
                "Wins: " + std::to_string(stat.nbrWins),
                "Score: " + std::to_string(stat.score)
            ));
        }
    });

    sceneManager.getNetwork().sendGetPlayerStatInRoom(roomId_);

    loaded = true;
}

void RoomScene::update() {
    refreshButton.setText(sceneManager.i18n_.get("refresh"));
    returnButton.setText(sceneManager.i18n_.get("return"));
}