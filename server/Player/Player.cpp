#include "Player.hpp"

Player::Player(int id, std::string ip) : id(id), ip(ip), tcp_port(0), udp_port(0), tcp_socket(nullptr), ping(0), lastPing(std::chrono::system_clock::now()) {}

Player::Player(int id, std::string ip, std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket)
    : id(id), ip(ip), tcp_port(0), udp_port(0), tcp_socket(tcp_socket) {}

Player::Player() : id(0), ip(""), tcp_port(0), udp_port(0), tcp_socket(nullptr) {}

Player::~Player() {}

int Player::getId() {
    return id;
}

std::string Player::getIp() {
    return ip;
}

unsigned short Player::getTCPPort() {
    return tcp_port;
}

unsigned short Player::getUDPPort() {
    return udp_port;
}

void Player::setIp(const std::string &ip) {
    this->ip = ip;
}

void Player::setId(int id) {
    this->id = id;
}

void Player::setTCPPort(unsigned short port) {
    this->tcp_port = port;
}

void Player::setUDPPort(unsigned short port) {
    this->udp_port = port;
}

void Player::setUDPRemote(std::shared_ptr<udp::endpoint> udp_remote) {
    this->udp_remote = udp_remote;
}

std::shared_ptr<udp::endpoint> Player::getUDPRemote() {
    return udp_remote;
}

std::shared_ptr<boost::asio::ip::tcp::socket> Player::getTCPSocket() {
    return tcp_socket;
}

void Player::setTCPSocket(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    this->tcp_socket = socket;
}