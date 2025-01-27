#include "RoomManager.hpp"

RoomManager::RoomManager() {
}

RoomManager::~RoomManager() {
}

std::vector<std::shared_ptr<Player>> RoomManager::getRoomPlayers(int roomId) {
    std::vector<std::shared_ptr<Player>> players;
    for (auto room : this->rooms) {
        if (room->getId() == roomId) {
            players = room->getPlayers();
            break;
        }
    }
    return players;
}

int RoomManager::getRoomIdByPlayerId(int playerId) {
    int roomId = -1;
    for (auto room : this->rooms) {
        for (auto player : room->getPlayers()) {
            if (player->getId() == playerId) {
                roomId = room->getId();
                break;
            }
        }
    }
    return roomId;
}

void RoomManager::playerJoinRoom(std::shared_ptr<Player> player, int roomId) {
    for (auto room : this->rooms) {
        std::cout << "Room id: " << room->getId() << std::endl;
        if (room->getId() == roomId) {
            if (room->getPlayers().size() != room->getMaxPlayers()) {
                room->addPlayer(player);
                std::cout << "Player " << player->getId() << " joined room " << roomId << "." << std::endl;
            } else {
                std::cout << "Room " << roomId << " is full." << std::endl;
            }
            break;
        }
    }
}

void RoomManager::playerLeaveRoom(int playerId) {
    for (auto room : this->rooms) {
        for (auto player : room->getPlayers()) {
            if (player->getId() == playerId) {
                room->removePlayer(player);
                std::cout << "Player " << playerId << " left room " << room->getId() << "." << std::endl;
                break;
            }
        }
    }
}

std::shared_ptr<Room> RoomManager::getRoom(int roomId) {
    std::shared_ptr<Room> room = nullptr;
    for (auto r : this->rooms) {
        if (r->getId() == roomId) {
            room = r;
            break;
        }
    }
    return room;
}

std::vector<std::shared_ptr<Room>> RoomManager::getRooms() {
    return this->rooms;
}

void RoomManager::addRoom(std::shared_ptr<Room> room) {
    this->rooms.push_back(room);
}

void RoomManager::removeRoom(std::shared_ptr<Room> room) {
    this->rooms.erase(std::remove(this->rooms.begin(), this->rooms.end(), room), this->rooms.end());
}

void RoomManager::dumpRoom(int roomId) {
    for (auto room : this->rooms) {
        if (room->getId() == roomId) {
            std::cout << "Room id: " << room->getId() << std::endl;
            std::cout << "Room name: " << room->getName() << std::endl;
            std::cout << "Room password: " << room->getPassword() << std::endl;
            std::cout << "Room state: " << room->getState() << std::endl;
            std::cout << "Room max players: " << room->getMaxPlayers() << std::endl;
            std::cout << "Room is " << (room->isPrivate() ? "private" : "public") << std::endl;
            std::cout << "Room players: " << std::endl;
            for (auto player : room->getPlayers()) {
                std::cout << "\tPlayer id: " << player->getId() << std::endl;
            }
            break;
        }
    }
}

void RoomManager::dumpRooms() {
    for (auto room : this->rooms) {
        std::cout << "------------------ Room (" << room->getId() << ") ------------------" << std::endl;
        std::cout << "Room id: " << room->getId() << std::endl;
        std::cout << "Room name: " << room->getName() << std::endl;
        std::cout << "Room password: " << room->getPassword() << std::endl;
        std::cout << "Room state: " << room->getState() << std::endl;
        std::cout << "Room max players: " << room->getMaxPlayers() << std::endl;
        std::cout << "Room is " << (room->isPrivate() ? "private" : "public") << std::endl;
        std::cout << "Room players: " << std::endl;
        for (auto player : room->getPlayers()) {
            std::cout << "\tPlayer id: " << player->getId() << std::endl;
        }
    }
}

int RoomManager::getRoomIncrement() {
    return this->roomIncrement;
}

int RoomManager::addRoomIncrement() {
    return ++this->roomIncrement;
}

bool RoomManager::hasPlayers(int room_id) const
{
    for (const auto& room : rooms) {
        if (room->getId() == room_id) {
            return !room->getPlayers().empty();
        }
    }
    return false;
}

void RoomManager::matchmaking(std::shared_ptr<Player> player)
{
    for (const auto& room : rooms) {
        if (room->getPlayers().size() < room->getMaxPlayers() && room->getState() == Room::State::WAITING && !room->isPrivate()) {
            playerJoinRoom(player, room->getId());
            std::cout << "Matchmaking: player " << player->getId() << " joined room " << room->getId() << std::endl;
            return;
        }
    }
    auto room = std::make_shared<Room>(addRoomIncrement(), "Room " + std::to_string(getRoomIncrement()), "", Room::State::WAITING, 4, false);
    addRoom(room);
    playerJoinRoom(player, room->getId());
    std::cout << "Matchmaking: player " << player->getId() << " joined room " << room->getId() << std::endl;
}