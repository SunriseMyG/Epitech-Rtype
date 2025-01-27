#include "Global.hpp"

GlobalScene::GlobalScene(SceneManager& sceneManager, FontLoader& fontLoader) 
    : Scene(sceneManager, fontLoader),
        chat_(fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sceneManager.getWindow()),
        refreshButton(sf::Vector2f(110, 110), sf::Vector2f(200, 50), sf::Color(128, 128, 128), sceneManager.i18n_.get("refresh"), fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"), sf::Color::White) {
}

void GlobalScene::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    for (auto& card : roomCards) {
        card.handleEvent(window, event, [&]() {
            std::cout << "Clicked on room " << card.id << std::endl;
            try {
                RoomScene& roomScene = dynamic_cast<RoomScene&>(sceneManager.getScene("room"));
                std::cout << "Successfully got RoomScene" << std::endl;
                roomScene.setRoomInfo(card.id, card.roomName, card.roomPassword, card.roomState, card.maxPlayers, card.currentPlayers);
                sceneManager.switchTo("room");
                std::cout << "Switched to room scene" << std::endl;
            } catch (const std::bad_cast& e) {
                std::cerr << "Error: Failed to cast to RoomScene: " << e.what() << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Error: Exception occurred: " << e.what() << std::endl;
            }
        });
    }

    refreshButton.handleEvent(window, event, [&]() {
        if (refreshCooldown.getElapsedTime().asSeconds() >= REFRESH_COOLDOWN_SECONDS) {
            sceneManager.getNetwork().sendGetRooms();
            refreshCooldown.restart();
        }
    });

    chat_.handleEvent(event, sceneManager.getWindow(), sceneManager.getNetwork());
}

void GlobalScene::loader() {
    sceneManager.getNetwork().setRoomsCallback([&](const std::vector<Room>& rooms) {
        roomCards.clear();
        
        const float CARD_WIDTH = 400;
        const float CARD_HEIGHT = 200;
        const float CARD_SPACING = 20;
        const float RIGHT_PADDING = -125;
        const float TOP_PADDING = 10;
        
        sf::Vector2u windowSize = sceneManager.getWindow().getSize();
        float maxVisibleHeight = windowSize.y - TOP_PADDING;
        
        for (size_t i = 0; i < rooms.size(); i++) {
            const auto& room = rooms[i];
            float xPos = windowSize.x - CARD_WIDTH - RIGHT_PADDING;
            float yPos = TOP_PADDING + 100 + (i * (CARD_HEIGHT + CARD_SPACING));
            
            if (yPos + CARD_HEIGHT > maxVisibleHeight)
                continue;
            
            roomCards.push_back(CreateCard(
                sf::Vector2f(xPos, yPos),
                sf::Vector2f(CARD_WIDTH, CARD_HEIGHT),
                sf::Color::White,
                fontLoader.getFont("VerminVibes1989Regular-m77m.ttf", "admin/Stream/VerminVibes1989Regular-m77m.ttf"),
                room.room_name + " (" + getStates(std::stoi(room.room_state)) + ")",
                std::to_string(room.current_players) + "/" + std::to_string(room.max_players),
                sceneManager.i18n_.get("information"),
                room.id, room.room_name, room.room_password, room.room_state, room.max_players, room.current_players
            ));
            // std::cout << "Created card for room ID: " << room.id << std::endl;
        }
    });

    sceneManager.getNetwork().sendGetRooms();
    loaded = true;
}

void GlobalScene::draw(sf::RenderWindow& window) {
    if (!loaded) {
        loader();
    }

    for (auto& card : roomCards) {
        card.draw(window);
    }

    chat_.draw(window);
    refreshButton.draw(window);
}

std::string GlobalScene::getStates(int number) {
    switch (number) {
        case 0:
            return "IN LOBBY";
        case 1:
            return "PLAYING";
        case 2:
            return "ENDED";
        default:
            return "UNKNOWN";
    }
}

void GlobalScene::update() {
    for (auto& card : roomCards) {
        card.setRightCornerButtonText(sceneManager.i18n_.get("information"));
    }

    refreshButton.setText(sceneManager.i18n_.get("refresh"));
}