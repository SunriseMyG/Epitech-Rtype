/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Events
*/
#include "../../include/Menu/Menu.hpp"
#include "../../include/ECS/Entity.hpp"
#include "../../include/EntityFactory.hpp"
#include <iostream>
#include <algorithm>
#include <thread>

void Menu::handleChatEvents(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (chatToggleButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isChatVisible = !isChatVisible;
            return;
        }
        if (isChatVisible) {
            isChatFocused = chatInput.getGlobalBounds().contains(mousePos.x, mousePos.y);
            chatInput.setOutlineColor(isChatFocused ? sf::Color(200, 200, 200) : sf::Color(150, 150, 150));
        }
    }
    
    if (isChatVisible && event.type == sf::Event::TextEntered && isChatFocused) {
        if (event.text.unicode == 8 && !chatInputString.empty()) {
            chatInputString.pop_back();
        } else if (event.text.unicode == 13 && !chatInputString.empty()) {
            std::string playerId = std::to_string(networkManager.getPlayerId());
            if (chatInputString.substr(0, 4) == "/msg") {
                size_t firstSpace = chatInputString.find(' ', 4);
                if (firstSpace != std::string::npos) {
                    if (firstSpace + 1 < chatInputString.length()) {
                        std::string targetUsername = chatInputString.substr(5, firstSpace - 5);
                        std::string privateMessage = chatInputString.substr(firstSpace + 1);
                        
                        // Debug
                        std::cout << "Target avant nettoyage: [" << targetUsername << "]" << std::endl;
                        std::cout << "Message avant nettoyage: [" << privateMessage << "]" << std::endl;
                        
                        // Nettoyer les espaces
                        targetUsername.erase(0, targetUsername.find_first_not_of(" \t\n\r\f\v"));
                        targetUsername.erase(targetUsername.find_last_not_of(" \t\n\r\f\v") + 1);
                        
                        privateMessage.erase(0, privateMessage.find_first_not_of(" \t\n\r\f\v"));
                        privateMessage.erase(privateMessage.find_last_not_of(" \t\n\r\f\v") + 1);
                        
                        // Debug après nettoyage
                        std::cout << "Target après nettoyage: [" << targetUsername << "]" << std::endl;
                        std::cout << "Message après nettoyage: [" << privateMessage << "]" << std::endl;
                        
                        if (!targetUsername.empty() && !privateMessage.empty()) {
                            networkManager.sendTCP("122 " + targetUsername + " " + privateMessage);
                            // Ne pas ajouter le message localement, attendre la réponse du serveur
                        } else {
                            addChatMessage("Système: Format incorrect. Utilisez /msg <username> <message>");
                        }
                    }
                }
            } else {
                std::string fullMessage = playerId + ": " + chatInputString;
                if (currentState == MenuState::ROOM_LOBBY) {
                    networkManager.sendTCP("123 " + fullMessage);
                    addChatMessage(fullMessage);
                } else {
                    networkManager.sendTCP("124 " + fullMessage);
                }
            }
            chatInputString.clear();
        } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            chatInputString += static_cast<char>(event.text.unicode);
        }
        chatInputText.setString(chatInputString);
    }
}

void Menu::addChatMessage(const std::string& message, bool isPrivate) {
    sf::Text chatMessage;
    chatMessage.setFont(robotoFont);
    chatMessage.setCharacterSize(16);
    
    std::string displayMessage = message;
    
    if (isPrivate) {
        size_t colonPos = displayMessage.find(":");
        if (colonPos != std::string::npos) {
            std::string sender = displayMessage.substr(0, colonPos);
            std::string remainingContent = displayMessage.substr(colonPos + 1);
            size_t lastSpacePos = remainingContent.rfind(" ");
            if (lastSpacePos != std::string::npos) {
                std::string actualMessage = remainingContent.substr(0, lastSpacePos);
                while (!actualMessage.empty() && std::isspace(actualMessage.front())) actualMessage.erase(0, 1);
                while (!actualMessage.empty() && std::isspace(actualMessage.back())) actualMessage.pop_back();
                
                displayMessage = "[MP] " + sender + ":" + actualMessage;
            } else {
                displayMessage = "[MP] " + sender + ":" + remainingContent;
            }
        }
    }
    
    chatMessage.setFillColor(isPrivate ? sf::Color(255, 215, 0) : sf::Color::White);
    chatMessage.setString(displayMessage);
    
    chatMessages.push_back(chatMessage);
    messageHistory.push_front(displayMessage);
    
    if (messageHistory.size() > MAX_CHAT_MESSAGES) {
        messageHistory.pop_back();
    }
    
    updateChatMessages();
}

void Menu::updateChatMessages() {
    chatMessages.clear();
    float yPos = chatBox.getPosition().y + chatBox.getSize().y - 30;
    
    for (const auto& message : messageHistory) {
        sf::Text messageText;
        messageText.setFont(robotoFont);
        messageText.setCharacterSize(16);
        messageText.setString(message);
        messageText.setPosition(chatBox.getPosition().x + 10, yPos);
        
        // Vérifier si c'est un message privé en cherchant le préfixe [MP]
        if (message.find("[MP]") != std::string::npos) {
            messageText.setFillColor(sf::Color(255, 215, 0));
        } else if (message.find(":") != std::string::npos) {
            std::string username = message.substr(0, message.find(":"));
            // Messages normaux en blanc
            messageText.setFillColor(sf::Color::White);
        } else {
            messageText.setFillColor(sf::Color::White);
        }
        
        chatMessages.push_back(messageText);
        yPos -= 25;
    }
}
