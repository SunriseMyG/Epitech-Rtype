/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Network
*/

#pragma once
#include <boost/asio.hpp>
#include <string>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <vector>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

struct Room {
    std::string id;
    std::string room_name;
    std::string room_password;
    std::string room_state;
    int max_players;
    int current_players;
};

struct Statistic {
    std::string player_id;
    int nbrDeaths;
    int nbrGames;
    int nbrKills;
    int nbrLosses;
    int nbrWins;
    int score;
};

class Network {
    public:
        Network(const std::string& host, const std::string& port);
        ~Network();
        void connect();
        void connectUDP(int player_id);
        void disconnect();
        bool isConnected() const { return connected; }
        void sendTCP(const std::string& message);


        void startReceiving();
        std::string getNextMessage();
        bool hasMessages() const;

        void handleUDPReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

        void setLoginCallback(std::function<void(bool)> callback);
        void setRoomsCallback(std::function<void(const std::vector<Room>&)> callback);
        void setMessageCallback(std::function<void(const std::string&)> callback);
        void setPlayerStatInRoomCallback(std::function<void(const std::vector<Statistic>&)> callback);
        std::vector<Room> getRooms() const { return rooms; }

        // command function
        void sendLogin(const std::string& username, const std::string& password);
        void sendGetRooms();
        void sendGetPlayerInRoom();
        void sendGetPlayerStatInRoom(const std::string& room_id);
        void sendKickPlayer(const std::string &id);
        void sendBanPlayer(const std::string &ban);

        std::vector<Room> stockRooms(const std::string& rooms);
        std::vector<Statistic> stockPlayerStatistics(const std::string& statistics);

        
    private:
        void receiveLoop();
        void handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred);

        boost::asio::io_context io_context;
        std::shared_ptr<tcp::socket> tcp_socket;
        udp::socket udp_socket;
        bool connected;
        std::string host;
        std::string port;
        udp::endpoint udp_server_endpoint;
        
        std::thread receive_thread;
        bool should_stop;
        
        std::array<char, 10240> receive_buffer;
        int player_id = -1;
        
        std::array<char, 10240> udp_receive_buffer;
        
        std::vector<Room> rooms;
        std::vector<Statistic> statistics;
        std::function<void(bool)> loginCallback;
        std::function<void(const std::vector<Room>&)> roomsCallback;
        std::function<void(const std::string&)> messageCallback;
        std::function<void(const std::vector<Statistic>&)> playerStatInRoomCallback;

};