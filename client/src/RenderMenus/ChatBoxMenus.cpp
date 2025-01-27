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

void Menu::createChat() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    
    chatToggleButton.setSize(sf::Vector2f(40, 40));
    chatToggleButton.setPosition(10, 10);
    chatToggleButton.setFillColor(sf::Color(70, 70, 70, 200));
    chatToggleButton.setOutlineThickness(2);
    chatToggleButton.setOutlineColor(sf::Color(100, 100, 100, 200));
    
    chatToggleText.setFont(font);
    chatToggleText.setString("Chat");
    chatToggleText.setCharacterSize(20);
    chatToggleText.setPosition(
        chatToggleButton.getPosition().x + (chatToggleButton.getSize().x - chatToggleText.getGlobalBounds().width) / 2,
        chatToggleButton.getPosition().y + (chatToggleButton.getSize().y - chatToggleText.getGlobalBounds().height) / 2
    );
    
    chatBox.setSize(sf::Vector2f(windowWidth * 0.3f, windowHeight * 0.3f));
    chatBox.setPosition(60, windowHeight * 0.6f);
    chatBox.setFillColor(sf::Color(30, 30, 30, 180));
    chatBox.setOutlineThickness(2);
    chatBox.setOutlineColor(sf::Color(100, 100, 100, 180));
    
    chatInput.setSize(sf::Vector2f(windowWidth * 0.3f, 30));
    chatInput.setPosition(60, windowHeight * 0.6f + chatBox.getSize().y + 5);
    chatInput.setFillColor(sf::Color(50, 50, 50, 200));
    chatInput.setOutlineThickness(2);
    chatInput.setOutlineColor(sf::Color(150, 150, 150, 200));
    
    chatInputText.setFont(font);
    chatInputText.setCharacterSize(16);
    chatInputText.setPosition(65, chatInput.getPosition().y + 5);
    
    isChatFocused = false;
    isChatVisible = false;
    chatInputString = "";
    
    updateChatMessages();
}


void Menu::renderChat() {
    window.draw(chatToggleButton);
    window.draw(chatToggleText);
    if (isChatVisible) {
        window.draw(chatBox);
        window.draw(chatInput);
        window.draw(chatInputText);
        
        for (const auto& message : chatMessages) {
            window.draw(message);
        }
    }
}