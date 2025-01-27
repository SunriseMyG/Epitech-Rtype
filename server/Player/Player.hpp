#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <boost/asio.hpp>
#include "IPlayer.hpp"
#include <iostream>
#include <memory>

class Player : public IPlayer {
    private:
        int id;
        std::string ip;
        unsigned short tcp_port;
        unsigned short udp_port;
        std::shared_ptr<udp::endpoint> udp_remote;
        std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket;
        int ping;
        std::chrono::time_point<std::chrono::high_resolution_clock> pingStartTime;
        std::chrono::time_point<std::chrono::system_clock> lastPing;

    public:
        Player(int id, std::string ip);
        Player(int id, std::string ip, std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket);
        Player(); // Constructeur par défaut
        ~Player();

        int getId() override;
        std::string getIp();
        unsigned short getTCPPort();
        unsigned short getUDPPort();
        void setIp(const std::string &ip);
        void setId(int id);
        void setTCPPort(unsigned short port);
        void setUDPPort(unsigned short port);
        void setUDPRemote(std::shared_ptr<udp::endpoint> udp_remote);
        std::shared_ptr<udp::endpoint> getUDPRemote();
        std::shared_ptr<boost::asio::ip::tcp::socket> getTCPSocket();
        void setTCPSocket(std::shared_ptr<boost::asio::ip::tcp::socket> socket);

        void setPing(int ping) { this->ping = ping; };
        int getPing() { return ping; };
        void startPing() { 
            this->pingStartTime = std::chrono::high_resolution_clock::now(); 
        };
        int stopPing() {
            auto endPing = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endPing - pingStartTime);
            return static_cast<int>(elapsed.count() / 1000.0);
        };

        void setLastPing() { this->lastPing = std::chrono::system_clock::now(); };
        std::chrono::system_clock::time_point getLastPing() { return lastPing; };
};

#endif // PLAYER_HPP
