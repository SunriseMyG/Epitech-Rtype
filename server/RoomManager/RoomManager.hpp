#ifndef RoomManager_HPP
#define RoomManager_HPP

#include "IRoomManager.hpp"

class RoomManager : public IRoomManager {
    public:
        RoomManager();
        ~RoomManager();

        //Players
        std::vector<std::shared_ptr<Player>> getRoomPlayers(int roomId) override;
        int getRoomIdByPlayerId(int playerId) override;
        void playerJoinRoom(std::shared_ptr<Player> player, int roomId) override;
        void playerLeaveRoom(int playerId) override;

        //Rooms
        std::shared_ptr<Room> getRoom(int roomId) override;
        std::vector<std::shared_ptr<Room>> getRooms() override;
        void addRoom(std::shared_ptr<Room> room) override;
        void removeRoom(std::shared_ptr<Room> room) override;
        void dumpRoom(int roomId) override;
        void dumpRooms() override;
        void matchmaking(std::shared_ptr<Player> player) override;

        //Increment
        int getRoomIncrement() override;
        int addRoomIncrement() override;

        bool hasPlayers(int room_id) const;

    private:
        std::vector<std::shared_ptr<Room>> rooms;
        int roomIncrement = 0;
};

#endif // RoomManager_HPP