#ifndef ROOM_HPP
    #define ROOM_HPP

    #include <SFML/Graphics.hpp>
    #include <string>
    #include "SceneManager.hpp"
    #include "../Utils/Chat.hpp"

    class RoomScene : public Scene {
        public:
            RoomScene(SceneManager& sceneManager, FontLoader& fontLoader);
            void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
            void draw(sf::RenderWindow& window) override;
            void setRoomInfo(const std::string& roomId, const std::string& roomName, const std::string& roomPassword, const std::string& roomState, int maxPlayers, int currentPlayers);
            void update() override;
            void loader();
        private:
            bool loaded = false;
            Chat chat_;
            CreateButton refreshButton;
            CreateButton returnButton;
            sf::Clock refreshCooldown;
            const float REFRESH_COOLDOWN_SECONDS = 2.0f;
            std::string roomId_;
            std::string roomName_;
            std::string roomPassword_;
            std::string roomState_;
            int maxPlayers_;
            int currentPlayers_;
            std::vector<CreateStatCard> playerStats;


            sf::RectangleShape roomInfoBox;
            CreateText* roomNameText;
            CreateText* roomPasswordText;
            CreateText* roomStateText;
            CreateText* maxPlayersText;
            CreateText* currentPlayersText;
        };

#endif // ROOM_HPP