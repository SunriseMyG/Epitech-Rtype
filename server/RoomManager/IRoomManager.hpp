#ifndef IRoomManager_HPP
#define IRoomManager_HPP

#include <vector>
#include <memory>
#include <string>
#include "../Room/Room.hpp"
#include <iostream>

class IRoomManager {
    public:
        virtual ~IRoomManager() = default;

        //Players
        virtual std::vector<std::shared_ptr<Player>> getRoomPlayers(int roomId) = 0;
        virtual int getRoomIdByPlayerId(int playerId) = 0;
        virtual void playerJoinRoom(std::shared_ptr<Player> player, int roomId) = 0;
        virtual void playerLeaveRoom(int playerId) = 0;

        //Rooms
        virtual std::shared_ptr<Room> getRoom(int roomId) = 0;
        virtual std::vector<std::shared_ptr<Room>> getRooms() = 0;
        virtual void addRoom(std::shared_ptr<Room> room) = 0;
        virtual void removeRoom(std::shared_ptr<Room> room) = 0;
        virtual void dumpRoom(int roomId) = 0;
        virtual void dumpRooms() = 0;
        virtual void matchmaking(std::shared_ptr<Player> player) = 0;

        //Increment
        virtual int getRoomIncrement() = 0;
        virtual int addRoomIncrement() = 0;
};

#endif // IRoomManager_HPP