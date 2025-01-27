#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

#include <boost/asio.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <array>
#include <algorithm>
#include "../Player/Player.hpp"
#include "../Room/Room.hpp"
#include "../RoomManager/RoomManager.hpp"
#include "../GameEngine/GameEngine.hpp"
#include "../Database/Database.hpp"
#include <thread>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Protocol {
public:
    Protocol(boost::asio::io_context& io_context, short port, bool disable_serialization = false);
    ~Protocol() = default;
    void tcpConnection();
    void receiveTCPMessage(std::shared_ptr<tcp::socket> socket_ptr);
    std::string prepareMessage(const std::string &message);
    void sendTCPMessage(const std::string &message, std::shared_ptr<tcp::socket> socket);
    void sendUDPMessage(const std::string &message, const udp::endpoint &remote_endpoint);
    void receiveUDPMessage();
    void handleUDP(std::shared_ptr<std::array<char, 1024>> buffer, std::shared_ptr<udp::endpoint> remote_endpoint, boost::system::error_code ec, std::size_t bytes_recvd);
    void handleClientDisconnect(std::shared_ptr<tcp::socket> socket_ptr);
    // TCP COMMAND
    void handleTCPCommand(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Connexion and return player id to client with code 100
    void handleCommand100(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Create a room and return OK or KO to client with code 102
    void handleCommand102(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Join a room and return OK or KO to client with code 103
    void handleCommand103(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get all rooms and return them to client with code 104
    void handleCommand104(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Start a game and return OK or KO to client with code 105
    void handleCommand105(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Leave a room and return OK or KO to client with code 112
    void handleCommand112(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Game ending and return OK or KO to all players in the room with code 113
    void handleCommand113(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Change the difficulty of the room and return OK or KO to client with code 114
    void handleCommand118(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get all players in the room and return them to client with code 118
    void handleCommand119(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Register a player and stock the data in database
    void handleCommand120(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Login a player and check if the player is already in the database
    void handleCommand121(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Send a private message to a player
    void handleCommand122(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Send a message to all players in the room
    void handleCommand123(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Send a message to all players in the main menu
    void handleCommand124(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get the leaderboard
    void handleCommand125(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Set the score of a player in the leaderboard
    void handleCommand126(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Join a room with matchmaking
    void handleCommand127(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get all statics of players
    void handleCommand129(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get stat of a player
    void handleCommand130(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get all player (username) connected on the server
    void handleCommand131(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get players in a room from room ID
    void handleCommand132(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Kick a player
    void handleCommand133(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Ban a player
    void handleCommand134(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get statistic of a player
    void handleCommand135(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // Get all rooms
    void handleCommand136(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    // UDP COMMAND
    void handleUDPCommand(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void dumpUDP(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand101(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand106(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand107(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand108(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand109(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand110(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand111(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand115(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);
    void handleCommand116(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint);

    std::vector<std::string> parseMessage(const std::string &message);
    std::vector<std::string> parseSentence(const std::string &sentence);

    RoomManager& getRoomManager() { return room_manager_; }

    bool banWordCheck(int code, const std::string& message, std::shared_ptr<tcp::socket> socket_ptr);

    std::string getEntitiesInfoAsJson(std::shared_ptr<Room> room);

    void pingTCP(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void pingLoop();
    void handleCommandPong(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr);
    bool verificationNextPing(Player *player);

private:
    int autoIncrementPlayerId();

    udp::socket udp_socket_;
    tcp::acceptor tcp_acceptor_;
    bool disable_serialization_;
    std::unordered_map<std::string, std::shared_ptr<Player>> players_;
    std::unordered_map<int, std::shared_ptr<Player>> players_by_id_;
    int next_player_id_;
    std::unordered_map<std::string, std::function<void(const std::string &, std::shared_ptr<tcp::socket>)>> tcp_command_handlers_;
    std::unordered_map<std::string, std::function<void(const std::string &, std::shared_ptr<udp::endpoint>)>> udp_command_handlers_;

    RoomManager room_manager_;

    GameEngine game_engine_;

    Database database_;

    // template <typename T>
    // void Protocol::sendTCPObject(const T &obj, std::shared_ptr<boost::asio::ip::tcp::socket> socket);
};

#endif // PROTOCOL_HPP