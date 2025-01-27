#include "Chat.hpp"

Chat::Chat(const sf::Font& font, const sf::RenderWindow& window)
    : font(font)
    , isActive(false)
    , scrollPosition(0.0f)
    , totalContentHeight(0.0f)
    , isDraggingScrollbar(false)
    // , inputField(
    //     sf::Vector2f(0, 0),  // Position will be set in setPosition
    //     sf::Vector2f(0, INPUT_HEIGHT),  // Width will be set in setSize
    //     font,
    //     "Type a message..."
    // )
{
    // Chat box dimensions
    const float CHAT_WIDTH = 400.f;
    const float CHAT_HEIGHT = 300.f;
    
    // Get window size
    sf::Vector2u windowSize = window.getSize();
    
    // Calculate center position
    sf::Vector2f position(
        10.f,
        windowSize.y - CHAT_HEIGHT - 10.f
    );
    
    chatBox.setSize(sf::Vector2f(CHAT_WIDTH, CHAT_HEIGHT));
    chatBox.setPosition(position);
    chatBox.setFillColor(sf::Color(50, 50, 50, 200));
    
    scrollTrack.setPosition(position.x + CHAT_WIDTH - SCROLLBAR_WIDTH, position.y);
    scrollTrack.setSize(sf::Vector2f(SCROLLBAR_WIDTH, CHAT_HEIGHT - INPUT_HEIGHT));
    scrollTrack.setFillColor(sf::Color(100, 100, 100));
    
    scrollbar.setPosition(scrollTrack.getPosition());
    scrollbar.setSize(sf::Vector2f(SCROLLBAR_WIDTH, CHAT_HEIGHT * 0.5f));
    scrollbar.setFillColor(sf::Color(150, 150, 150));
    
    setPosition(position);
    setSize(sf::Vector2f(CHAT_WIDTH, CHAT_HEIGHT));
    updateMessageDisplay();
}

void Chat::handleEvent(const sf::Event& event, sf::RenderWindow& window, Network &network) {
    if (event.type == sf::Event::MouseWheelScrolled) {
        if (chatBox.getGlobalBounds().contains(event.mouseWheelScroll.x, event.mouseWheelScroll.y)) {
            handleScroll(event.mouseWheelScroll.delta);
        }
    }
    
    if (event.type == sf::Event::MouseButtonPressed) {
        if (chatBox.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            isActive = true;
            if (scrollbar.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                isDraggingScrollbar = true;
            }
        } else {
            isActive = false;
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        isDraggingScrollbar = false;
    }
    
    if (event.type == sf::Event::MouseMoved && isDraggingScrollbar) {
        handleScrollbarDrag(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
    }
    
    // inputField.handleEvent(event, window);
    
    // if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
    //     std::string message = inputField.getText();
    //     if (!message.empty()) {
    //         addMessage(message);
    //         inputField.setPlaceholder("Type a message...");
    //     }
    // }

    network.setMessageCallback([&](const std::string& message) {
        std::cout << "Message received in callback " << message << std::endl;
        addMessage(message);
    });
}

void Chat::addMessage(const std::string& message) {
    messages.push_front(message);
    if (messages.size() > MAX_MESSAGES) {
        messages.pop_back();
    }
    updateMessageDisplay();
}

void Chat::updateMessageDisplay() {
    messageTexts.clear();
    float yPos = chatBox.getPosition().y + chatBox.getSize().y - INPUT_HEIGHT - MESSAGE_PADDING;
    
    for (const auto& msg : messages) {
        sf::Text text;
        text.setFont(font);
        text.setString(msg);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        text.setPosition(chatBox.getPosition().x + MESSAGE_PADDING, 
                        yPos - text.getGlobalBounds().height - scrollPosition);
        messageTexts.push_back(text);
        yPos -= text.getGlobalBounds().height + MESSAGE_PADDING;
    }
    
    totalContentHeight = (messageTexts.size() * (14 + MESSAGE_PADDING));
}

void Chat::draw(sf::RenderWindow& window) {
    window.draw(chatBox);
    // inputField.draw(window);
    window.draw(scrollTrack);
    window.draw(scrollbar);
    
    for (const auto& text : messageTexts) {
        if (text.getPosition().y >= chatBox.getPosition().y && 
            text.getPosition().y <= chatBox.getPosition().y + chatBox.getSize().y - INPUT_HEIGHT) {
            window.draw(text);
        }
    }
}


void Chat::setPosition(const sf::Vector2f& position) {
    chatBox.setPosition(position);
    // inputField.setPosition(sf::Vector2f(
    //     position.x + MESSAGE_PADDING,
    //     position.y + chatBox.getSize().y - INPUT_HEIGHT - MESSAGE_PADDING
    // ));
    scrollTrack.setPosition(position.x + chatBox.getSize().x - SCROLLBAR_WIDTH, position.y);
    updateMessageDisplay();
}


void Chat::setSize(const sf::Vector2f& size) {
    chatBox.setSize(size);
    // inputField.setSize(sf::Vector2f(size.x - 2 * MESSAGE_PADDING - SCROLLBAR_WIDTH, INPUT_HEIGHT));
    scrollTrack.setSize(sf::Vector2f(SCROLLBAR_WIDTH, size.y - INPUT_HEIGHT));
    
    float scrollbarHeight = (size.y / totalContentHeight) * size.y;
    scrollbar.setSize(sf::Vector2f(SCROLLBAR_WIDTH, scrollbarHeight));
    updateMessageDisplay();
}

void Chat::updateScrollPosition(float newPosition) {
    float maxScroll = std::max(0.0f, totalContentHeight - chatBox.getSize().y + INPUT_HEIGHT);
    scrollPosition = std::max(0.0f, std::min(newPosition, maxScroll));
    
    float scrollbarY = (scrollPosition / maxScroll) * 
                      (chatBox.getSize().y - INPUT_HEIGHT - scrollbar.getSize().y);
    scrollbar.setPosition(scrollTrack.getPosition().x,
                         scrollTrack.getPosition().y + scrollbarY);
    
    updateMessageDisplay();
}

void Chat::handleScroll(float delta) {
    updateScrollPosition(scrollPosition - (delta * 20.0f));
}

void Chat::handleScrollbarDrag(const sf::Vector2f& mousePos) {
    float relativeY = mousePos.y - scrollTrack.getPosition().y;
    float scrollPercent = relativeY / (scrollTrack.getSize().y - scrollbar.getSize().y);
    float newScrollPos = scrollPercent * (totalContentHeight - chatBox.getSize().y + INPUT_HEIGHT);
    updateScrollPosition(newScrollPos);
}