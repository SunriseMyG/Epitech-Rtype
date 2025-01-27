/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** NetworkManager
*/

#include "../include/Network/NetworkManager.hpp"
#include "../include/Parser/jsonParser.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>

NetworkManager::NetworkManager(const std::string& h, const std::string& p)
    : tcp_socket(std::make_shared<tcp::socket>(io_context)), 
      udp_socket(io_context), 
      connected(false), 
      host(h), 
      port(p), 
      should_stop(false) {}

NetworkManager::~NetworkManager() {
    should_stop = true;
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    disconnect();
}

void NetworkManager::connect() {
    try {
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);
        boost::asio::connect(*tcp_socket, endpoints);

        udp::resolver udp_resolver(io_context);
        udp_server_endpoint = *udp_resolver.resolve(udp::v4(), host, port).begin();
        udp_socket.open(udp::v4());
        connected = true;
        std::cout << "Connected to server" << std::endl;
        
        receive_thread = std::thread([this]() {
            try {
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "Error in io_context thread: " << e.what() << std::endl;
            }
        });
        
        startReceiving();
        startReceivingUDP();
        
    } catch (std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        throw;
    }
}

void NetworkManager::connectUDP(int player_id) {
    if (connected) {
        udp_socket.send_to(boost::asio::buffer("101 " + std::to_string(player_id)), udp_server_endpoint);
        std::cout << "Envoi de la commande de connexion UDP: " << "101 " + std::to_string(player_id) << std::endl;
    }
}

void NetworkManager::sendUDP(const std::string& message) {
    if (connected) {
        udp_socket.send_to(boost::asio::buffer(message), udp_server_endpoint);
    }
}

void NetworkManager::sendTCP(const std::string& message) {
    if (!connected || !tcp_socket) {
        std::cerr << "Not connected or socket is null" << std::endl;
        return;
    }

    try {
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << message;
        std::string serialized_data = oss.str();
        boost::asio::write(*tcp_socket, boost::asio::buffer(serialized_data));
    } catch (const std::exception& e) {
        std::cerr << "Error sending TCP message: " << e.what() << std::endl;
    }
}

void NetworkManager::disconnect() {
    if (connected) {
        tcp_socket->close();
        udp_socket.close();
        connected = false;
    }
}

void NetworkManager::startReceiving() {
    if (!connected) return;
    
    if (!io_context.stopped()) {
        tcp_socket->async_read_some(
            boost::asio::buffer(receive_buffer),
            [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
                handleReceive(error, bytes_transferred);
            }
        );
    } else {
        io_context.restart();
        startReceiving();
    }
}

void NetworkManager::handleReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
        message_buffer.append(receive_buffer.data(), bytes_transferred);
        std::string message = message_buffer.extractMessage();

        std::ostringstream full_message;
        full_message.write(receive_buffer.data(), bytes_transferred);
        
        std::istringstream iss(full_message.str());
        boost::archive::text_iarchive ia(iss);
        try {
            ia >> message;
        } catch (const boost::archive::archive_exception& e) {
            std::cerr << "Erreur d'archive Boost: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erreur générique: " << e.what() << std::endl;
        }
        
        if (!message.empty()) {
            if (message == "*P") {
                return;
            }
            
            // Extraction de l'ID du protocole
            if (message[0] == '$') {
                std::string protocol_id = message.substr(1, 3);
                std::string content = message.substr(5);
                if (message.substr(1, 4) == "PING") { // PING PONG
                    sendTCP("PONG");
                } else if (protocol_id == "103" || protocol_id == "127") {
                    std::cout << "Message reçu room select: " << message << std::endl;
                    std::lock_guard<std::mutex> lock(room_select_queue_mutex);
                    room_select_message_queue.push(message);
                } else if (protocol_id == "104") {  // Room list
                    std::cout << "Message reçu room list: " << message << std::endl;
                    std::lock_guard<std::mutex> lock(room_list_queue_mutex);
                    room_list_message_queue.push(message);
                } else if (protocol_id == "105") {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(message);
                } else if (protocol_id == "119" || protocol_id == "112") {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(message);
                } else if (protocol_id == "125") {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(message);
                } else if (protocol_id == "114") {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    message_queue.push(message);
                } else if (protocol_id == "123" || protocol_id == "124" || protocol_id == "122") {
                    std::lock_guard<std::mutex> lock(queue_mutex);
                    bool isPrivate = (protocol_id == "122");
                    message_queue.push("$" + protocol_id + " " + content);
                } else if (protocol_id == "120") {
                    if (message.find("OK") != std::string::npos) {
                        size_t firstSpace = message.find(' ', 5);
                        if (firstSpace != std::string::npos) {
                            std::string idStr = message.substr(firstSpace + 1);
                            size_t secondSpace = idStr.find(' ');
                            if (secondSpace != std::string::npos) {
                                idStr = idStr.substr(0, secondSpace);
                                try {
                                    player_id = std::stoi(idStr);
                                    connectUDP(player_id);
                                    requestRooms();
                                } catch (const std::exception& e) {
                                    std::cerr << "Erreur lors de la conversion du player_id: " << e.what() << std::endl;
                                }
                            }
                        }
                    }
                    std::lock_guard<std::mutex> lock(login_queue_mutex);
                    login_message_queue.push(message);
                } else if (protocol_id == "121") {
                    if (message.find("OK") != std::string::npos) {
                        size_t firstSpace = message.find(' ', 5);
                        if (firstSpace != std::string::npos) {
                            std::string idStr = message.substr(firstSpace + 1);
                            size_t secondSpace = idStr.find(' ');
                            if (secondSpace != std::string::npos) {
                                idStr = idStr.substr(0, secondSpace);
                                try {
                                    player_id = std::stoi(idStr);
                                    connectUDP(player_id);
                                    requestRooms();
                                } catch (const std::exception& e) {
                                    std::cerr << "Erreur lors de la conversion du player_id: " << e.what() << std::endl;
                                }
                            }
                        }
                    }
                    std::lock_guard<std::mutex> lock(login_queue_mutex);
                    login_message_queue.push(message);
                }
            } 
        }
        
        startReceiving();
    } else if (error) {
        std::cerr << "Erreur dans handleReceive TCP (Player ID: " << player_id << "): " << error.message() << std::endl;
        if (error != boost::asio::error::operation_aborted) {
            startReceiving();
        }
    }
}

void NetworkManager::handleUDPReceive(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
        udp_message_buffer.append(udp_receive_buffer.data(), bytes_transferred);
        std::string message = udp_message_buffer.extractMessage();

        std::ostringstream full_message;
        full_message.write(udp_receive_buffer.data(), bytes_transferred);

        std::istringstream iss(full_message.str());
        boost::archive::text_iarchive ia(iss);
        try {
            ia >> message;
        } catch (const boost::archive::archive_exception& e) {
            std::cerr << "Erreur d'archive Boost: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erreur générique: " << e.what() << std::endl;
        }
        
        if (!message.empty() && message[0] == '$') {
            std::string protocol_id = message.substr(1, 3);
            std::string content = message.substr(5);
            
            std::lock_guard<std::mutex> lock(udp_queue_mutex);
            
            if (protocol_id == "115") {
                game_state_queue.push(message);
            } else if (protocol_id == "107") {
                std::lock_guard<std::mutex> lock(action_queue_mutex);
                action_queue.push(message);
            }
        } else if (!message.empty()) {
            std::cout << "Message UDP reçu: " << message << std::endl;
        }
        startReceivingUDP();
    } else if (error) {
        std::cerr << "Erreur dans handleReceive UDP: " << error.message() << std::endl;
        if (error != boost::asio::error::operation_aborted) {
            startReceivingUDP();
        }
    }
}

void NetworkManager::startReceivingUDP() {
    if (!connected) return;

    udp_socket.async_receive_from(
        boost::asio::buffer(udp_receive_buffer),
        udp_sender_endpoint,
        [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
            handleUDPReceive(error, bytes_transferred);
        }
    );
}

std::string NetworkManager::getNextMessage(bool pop) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (message_queue.empty()) {
        return "";
    }
    std::string message = message_queue.front();
    std::string protocol_id = message.substr(1, 3);
    message_queue.pop();
    return message;
}

bool NetworkManager::hasMessages() const {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return !message_queue.empty();
}

void NetworkManager::requestRooms() {
    if (connected) {
        std::cout << "Envoi de la requête de rooms" << std::endl;
        sendTCP("104");
    } else {
        std::cerr << "Non connecté au serveur" << std::endl;
    }
}

void NetworkManager::joinRoom(const std::string& roomId, const std::string& password) {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    
    try {
        std::string command = "103 " + roomId;
        if (!password.empty()) {
            command += " " + password;
        }
        std::cout << "Envoi de la commande de jointure: " << command << std::endl;
        sendTCP(command);
        m_isInRoom = true;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la jointure de la room: " << e.what() << std::endl;
    }
}

void NetworkManager::leaveRoom(const std::string& roomId) {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    
    try {
        std::string command = "112";
        sendTCP(command);
        m_isInRoom = false;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la sortie de la room: " << e.what() << std::endl;
    }
}

void NetworkManager::startGame(const std::string& roomId) {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    
    try {
        std::string command = "105";
        sendTCP(command);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du démarrage de la partie: " << e.what() << std::endl;
    }
}

void NetworkManager::endGame(const std::string& roomId) {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    
    try {
        std::string command = "114";
        sendTCP(command);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la fin de la partie: " << e.what() << std::endl;
    }
}

std::string NetworkManager::getNextLoginMessage() {
    std::lock_guard<std::mutex> lock(login_queue_mutex);
    if (login_message_queue.empty()) {
        return "";
    }
    std::string message = login_message_queue.front();
    login_message_queue.pop();
    return message;
}

void NetworkManager::requestPlayersInRoom() {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    try {
        std::string command = "119";
        sendTCP(command);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la récupération des joueurs: " << e.what() << std::endl;
    }
}

std::string NetworkManager::getNextUDPMessage() {
    std::lock_guard<std::mutex> lock(udp_queue_mutex);
    if (udp_message_queue.empty()) {
        return "";
    }
    std::string message = udp_message_queue.front();
    udp_message_queue.pop();
    return message;
}


std::string NetworkManager::getNextRoomMessage() {
    std::lock_guard<std::mutex> lock(room_select_queue_mutex);
    if (room_select_message_queue.empty()) {
        return "";
    }
    std::string message = room_select_message_queue.front();
    room_select_message_queue.pop();
    return message;
}

std::string NetworkManager::getNextRoomListMessage() {
    std::lock_guard<std::mutex> lock(room_list_queue_mutex);
    if (room_list_message_queue.empty()) {
        return "";
    }
    std::string message = room_list_message_queue.front();
    room_list_message_queue.pop();
    return message;
}

bool NetworkManager::hasUDPMessages() const {
    std::lock_guard<std::mutex> lock(udp_queue_mutex);
    return !udp_message_queue.empty();
}

void NetworkManager::requestLeaderboard() {
    if (!connected) {
        std::cerr << "Non connecté au serveur" << std::endl;
        return;
    }
    
    try {
        std::string command = "125";
        sendTCP(command);
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la récupération du leaderboard: " << e.what() << std::endl;
    }
}

std::string NetworkManager::getNextGameStateMessage() {
    std::lock_guard<std::mutex> lock(udp_queue_mutex);
    if (game_state_queue.empty()) return "";
    std::string msg = game_state_queue.front();
    game_state_queue.pop();
    return msg;
}

std::string NetworkManager::getNextActionMessage() {
    std::lock_guard<std::mutex> lock(udp_queue_mutex);
    if (action_queue.empty()) return "";
    std::string msg = action_queue.front();
    action_queue.pop();
    return msg;
}

void NetworkManager::loginfunc(const std::string& username, const std::string& password) {
    std::string command = "121 " + username + " " + password;
    sendTCP(command);
}

void NetworkManager::registerfunc(const std::string& username, const std::string& password) {
    std::string command = "120 " + username + " " + password;
    sendTCP(command);
}
