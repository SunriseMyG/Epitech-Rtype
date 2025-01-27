/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** jsonParser
*/
#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

class JsonParser {
public:
    struct Room {
        std::string id;
        std::string name;
        std::string password;
        std::string maxPlayers;
        std::string currentPlayers;
        std::string state;
    };

    struct Player {
        std::string id;
        std::string name;
    };

    struct PlayerUDPposition {
        std::string id;
        float x;
        float y;
        std::string state;
        std::string group;
        std::string type;
        int health;
        int score;
    };

    struct EnemyUDPposition {
        std::string id;
        float x;
        float y;
        int health;
        bool isBoss;
        std::string type;
    };

    struct Leaderboard {
        std::string name;
        int score;
    };

    struct EntityPosition {
        int id;
        float x;
        float y;
        int score;
        int health;
    };

    explicit JsonParser(std::string_view jsonData) : data(jsonData) {}

    std::vector<Room> parseRooms() {
        std::vector<Room> rooms;
        try {
            json j = json::parse(data);
            
            for (const auto& [id, room_data] : j.items()) {
                Room room;
                room.id = id;
                room.name = room_data.value("room name", "");
                room.password = room_data.value("room password", "");
                room.state = room_data.value("room state", "");
                room.maxPlayers = room_data.value("max players", "");
                room.currentPlayers = room_data.value("current players", "");
                
                if (!room.name.empty()) {
                    rooms.push_back(room);
                }
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing JSON: " << e.what() << std::endl;
        }
        return rooms;
    }

    std::vector<std::string> parsePlayers() {
        std::vector<std::string> players;
        try {
            json j = json::parse(data);
            for (const auto& player : j) {
                players.push_back(player.get<std::string>());
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing JSON: " << e.what() << std::endl;
        }
        return players;
    }


    std::vector<PlayerUDPposition> parsePlayerUDPposition() {
        std::vector<PlayerUDPposition> players;
        try {
            json j = json::parse(data);
            for (const auto& [id, pos] : j.items()) {
                PlayerUDPposition playerPos;
                playerPos.id = id;
                playerPos.x = pos["x"].get<float>();
                playerPos.y = pos["y"].get<float>();
                players.push_back(playerPos);
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing JSON: " << e.what() << std::endl;
        }
        return players;
    }

    std::vector<EnemyUDPposition> parseEnemyUDPposition() {
        std::vector<EnemyUDPposition> enemies;
        try {
            json j = json::parse(data);
            for (const auto& [id, enemy] : j.items()) {
                EnemyUDPposition enemyPos;
                enemyPos.id = id;
                enemyPos.x = enemy["x"].get<float>();
                enemyPos.y = enemy["y"].get<float>();
                enemyPos.health = enemy["health"].get<int>();
                enemyPos.isBoss = enemy["BOSS"].get<bool>();
                enemyPos.type = enemy["type"].get<std::string>();
                enemies.push_back(enemyPos);
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing JSON ennemis: " << e.what() << std::endl;
        }
        return enemies;
    }

    std::vector<Leaderboard> parseLeaderboard() {
        std::vector<Leaderboard> leaderboard;
        try {
            // Trouver le début et la fin du JSON dans la chaîne
            size_t start = data.find("[");
            size_t end = data.find_last_of("]") + 1;
            
            if (start != std::string::npos && end != std::string::npos) {
                std::string jsonStr = std::string(data.substr(start, end - start));
                json j = json::parse(jsonStr);
                
                for (const auto& entry : j) {
                    if (entry.contains("username") && entry.contains("score")) {
                        std::string username = entry["username"].get<std::string>();
                        int score = entry["score"].get<int>();
                        if (!username.empty()) {
                            leaderboard.push_back({username, score});
                        }
                    }
                }
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing JSON leaderboard: " << e.what() << std::endl;
            std::cerr << "Data reçue: " << data << std::endl;
        }
        return leaderboard;
    }

    std::vector<EntityPosition> parseEntityPositions() {
        std::vector<EntityPosition> positions;
        try {
            json j = json::parse(data);
            for (const auto& [id, playerData] : j.items()) {
                EntityPosition pos;
                pos.id = std::stoi(id);
                pos.x = playerData["x"].get<float>();
                pos.y = playerData["y"].get<float>();
                pos.score = playerData["score"].get<int>();
                pos.health = playerData["health"].get<int>();
                positions.push_back(pos);
            }
        } catch (const json::parse_error& e) {
            std::cerr << "Erreur de parsing des positions: " << e.what() << std::endl;
        }
        return positions;
    }

    bool validatePlayerPosition(const PlayerUDPposition& pos) {
        return pos.x >= 0 && pos.x < 1920 &&  // Ajustez selon la taille de votre fenêtre
               pos.y >= 0 && pos.y < 1080;    // Ajustez selon la taille de votre fenêtre
    }

private:
    std::string_view data;
};