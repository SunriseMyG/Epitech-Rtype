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

void Menu::createScoreboard() {
    float windowWidth = window.getSize().x;
    float windowHeight = window.getSize().y;
    scoreboardBackground.setSize(sf::Vector2f(windowWidth * 0.20f, windowHeight * 0.6f));
    scoreboardBackground.setPosition(windowWidth * 0.82f, windowHeight * 0.2f);
    scoreboardBackground.setFillColor(sf::Color(30, 30, 30, 200));
    scoreboardBackground.setOutlineThickness(2);
    scoreboardBackground.setOutlineColor(sf::Color(100, 100, 100));
    scoreboardTitle.setFont(font);
    scoreboardTitle.setString("Classement");
    scoreboardTitle.setCharacterSize(windowHeight * 0.025f);
    scoreboardTitle.setFillColor(sf::Color::White);
    
    sf::FloatRect titleBounds = scoreboardTitle.getLocalBounds();
    scoreboardTitle.setPosition(
        windowWidth * 0.8f + (scoreboardBackground.getSize().x - titleBounds.width) / 2,
        windowHeight * 0.22f
    );
    isScoreboardVisible = true;
}

void Menu::updateScoreboard() {
    if (!isScoreboardVisible) return;
    networkManager.requestLeaderboard();
}

void Menu::updateScoreboardEntries(const std::vector<std::pair<std::string, int>>& scores) {
    float startY = window.getSize().y * 0.28f;
    float spacing = window.getSize().y * 0.04f;
    scoreboardEntries.clear();
    
    auto sortedScores = scores;

    std::sort(sortedScores.begin(), sortedScores.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });
    
    size_t numEntries = std::min(sortedScores.size(), size_t(10));
    
    for (size_t i = 0; i < numEntries; ++i) {
        sf::Text entry;
        entry.setFont(font);
        entry.setCharacterSize(window.getSize().y * 0.018f);
        entry.setFillColor(sf::Color::White);
        
        std::string text = std::to_string(i + 1) + ". " + 
                          sortedScores[i].first + " - " + 
                          std::to_string(sortedScores[i].second);
        entry.setString(text);
        
        entry.setPosition(
            scoreboardBackground.getPosition().x + 20,
            startY + (i * spacing)
        );
        
        scoreboardEntries.push_back(entry);
    }
}