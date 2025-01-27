#ifndef CHAT_HPP_
#define CHAT_HPP_

#include <SFML/Graphics.hpp>
#include <deque>
#include <string>
#include <vector>
#include <iostream>
#include "../Utils/FontLoader.hpp"
#include "../Utils/CreateInputField.hpp"
#include "../Network/Network.hpp"

class Chat {
    public:
        Chat(const sf::Font& font, const sf::RenderWindow& window);
        void handleEvent(const sf::Event& event, sf::RenderWindow& window, Network &network);
        void addMessage(const std::string& message);
        void draw(sf::RenderWindow& window);
        void setPosition(const sf::Vector2f& position);
        void setSize(const sf::Vector2f& size);

    private:
        static const size_t MAX_MESSAGES = 15;
        static constexpr float MESSAGE_PADDING = 5.0f;
        static constexpr float INPUT_HEIGHT = 30.0f;
        static constexpr float SCROLLBAR_WIDTH = 10.0f;
        
        std::deque<std::string> messages;
        bool isActive;
        
        sf::RectangleShape chatBox;
        // CreateInputField inputField;
        std::vector<sf::Text> messageTexts;
        
        // Scrollbar components
        sf::RectangleShape scrollbar;
        sf::RectangleShape scrollTrack;
        float scrollPosition;
        float totalContentHeight;
        bool isDraggingScrollbar;
        
        const sf::Font& font;
        
        void updateMessageDisplay();
        void processInput();
        void updateScrollPosition(float newPosition);
        void handleScroll(float delta);
        void handleScrollbarDrag(const sf::Vector2f& mousePos);
};

#endif // CHAT_HPP_