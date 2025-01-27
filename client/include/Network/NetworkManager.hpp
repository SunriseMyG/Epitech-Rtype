/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** NetworkManager
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

class NetworkManager {
    public:
        NetworkManager(const std::string& host, const std::string& port);
        ~NetworkManager();
        void connect();
        void connectUDP(int player_id);
        void disconnect();
        bool isConnected() const { return connected; }
        void sendUDP(const std::string& message);
        void sendTCP(const std::string& message);


        void startReceiving();
        std::string getNextMessage(bool pop = true);
        bool hasMessages() const;

        std::string getNextUDPMessage();
        std::string getNextGameStateMessage();
        std::string getNextActionMessage();
        std::string getNextLoginMessage();
        std::string getNextRoomMessage();
        std::string getNextRoomListMessage();
        bool hasRoomMessages() const {return !room_select_message_queue.empty(); }
        bool hasUDPMessages() const;
        bool hasActionMessages() const { return !action_queue.empty(); }
        bool hasGameStateMessages() const { return !game_state_queue.empty(); }
        bool hasRoomListMessages() const { return !room_list_message_queue.empty(); }
        bool hasLoginMessages() const { return !login_message_queue.empty(); }
        void requestRooms();
        void requestLeaderboard();
        int getPlayerId() const { return player_id; }
        void joinRoom(const std::string& roomId, const std::string& password);
        void leaveRoom(const std::string& roomId);
        void startGame(const std::string& roomId);
        void endGame(const std::string& roomId);
        void requestPlayersInRoom();

        void loginfunc(const std::string& username, const std::string& password);
        void registerfunc(const std::string& username, const std::string& password);

        void startReceivingUDP();
        void handleUDPReceive(const boost::system::error_code& error, std::size_t bytes_transferred);
        bool isInRoom() const { return m_isInRoom; }
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

        std::queue<std::string> room_select_message_queue;
        std::queue<std::string> message_queue;
        std::queue<std::string> login_message_queue;
        std::queue<std::string> udp_message_queue;
        std::queue<std::string> room_list_message_queue;
        std::queue<std::string> action_queue;  // Pour protocole 107

        mutable std::mutex room_select_queue_mutex;
        mutable std::mutex queue_mutex;
        mutable std::mutex udp_queue_mutex;
        mutable std::mutex login_queue_mutex;
        mutable std::mutex room_list_queue_mutex;
        mutable std::mutex action_queue_mutex;
        bool should_stop;
        bool m_isInRoom = false;
        
        std::array<char, 1024> receive_buffer;
        std::string receive_buffer_str;
        int player_id = -1;
        
        struct MessageBuffer {
            std::vector<char> data;
            size_t position;
            
            MessageBuffer() : position(0) {}
            
            void append(const char* new_data, size_t length) {
                data.insert(data.end(), new_data, new_data + length);
            }
            
            std::string extractMessage() {
                auto newline = std::find(data.begin() + position, data.end(), '\n');
                if (newline == data.end()) return "";
                
                std::string message(data.begin() + position, newline);
                position = newline - data.begin() + 1;
                
                if (position >= data.size()) {
                    data.clear();
                    position = 0;
                }
                
                return message;
            }
        };
        
        MessageBuffer message_buffer;
        
        std::array<char, 8192> udp_receive_buffer;
        MessageBuffer udp_message_buffer;
        
        udp::endpoint udp_sender_endpoint;

        std::queue<std::string> game_state_queue;  // Pour protocole 115    // Pour protocole 107

};