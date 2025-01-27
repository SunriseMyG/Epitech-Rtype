/*
** EPITECH PROJECT, 2024
** B-CPP-500-NCE-5-2-rtype-steven.deffontaine
** File description:
** Network
*/

#include "Network.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Network::Network(const std::string& h, const std::string& p)
    : tcp_socket(std::make_shared<tcp::socket>(io_context)), 
      udp_socket(io_context), 
      connected(false), 
      host(h), 
      port(p), 
      should_stop(false) {}

Network::~Network() {
    should_stop = true;
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    disconnect();
}

void Network::connect()
{
    try {
        std::cout << "Resolving TCP endpoints..." << std::endl;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(host, port);
        std::cout << "Connecting to TCP server..." << std::endl;
        boost::asio::connect(*tcp_socket, endpoints);

        std::cout << "Resolving UDP endpoints..." << std::endl;
        udp::resolver udp_resolver(io_context);
        udp_server_endpoint = *udp_resolver.resolve(udp::v4(), host, port).begin();
        std::cout << "Opening UDP socket..." << std::endl;
        udp_socket.open(udp::v4());

        connected = true;
        std::cout << "Connected to server" << std::endl;

        receive_thread = std::thread([this]() {
            try {
                std::cout << "Starting io_context run loop..." << std::endl;
                io_context.run();
            } catch (const std::exception& e) {
                std::cerr << "Error in io_context thread: " << e.what() << std::endl;
            }
        });


        startReceiving(); // Assurez-vous que startReceiving est appelé après la connexion

    } catch (std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        throw;
    }
}

void Network::sendTCP(const std::string& message) {
    
    if (!connected || !tcp_socket) {
        std::cerr << "Not connected or socket is null" << std::endl;
        return;
    }
    try {
        // serialize the message
        std::ostringstream oss;
        boost::archive::text_oarchive oa(oss);
        oa << message;
        std::string serialized_data = oss.str();
        boost::asio::write(*tcp_socket, boost::asio::buffer(serialized_data));
        // std::cout << "TCP message sent: " << message << std::endl;
        
        // non serialized message
        // boost::asio::write(*tcp_socket, boost::asio::buffer(message));
        // std::cout << "TCP message sent: " << message << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error sending TCP message: " << e.what() << std::endl;
    }
}

void Network::disconnect() {
    if (connected) {
        tcp_socket->close();
        udp_socket.close();
        connected = false;
    }
}

void Network::startReceiving() {
    // std::cout << "Starting to receive data..." << std::endl;
    receive_buffer.fill(0);
    tcp_socket->async_receive(boost::asio::buffer(receive_buffer),
        [this](boost::system::error_code ec, std::size_t bytes_transferred) {
            // std::cout << "Bytes received: " << bytes_transferred << std::endl;
            handleReceive(ec, bytes_transferred);
        });
}

void Network::handleReceive(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    if (ec) {
        std::cerr << "Error in receiving data: " << ec.message() << std::endl;
        return;
    }

    // std::cout << "Bytes received: " << bytes_transferred << std::endl;
    if (bytes_transferred == 0) {
        std::cerr << "No data received" << std::endl;
        return;
    }

    // std::string received_message(receive_buffer.begin(), receive_buffer.begin() + bytes_transferred);

    // serialize the message
    std::ostringstream full_message;
    full_message.write(receive_buffer.data(), bytes_transferred);
    
    std::istringstream iss(full_message.str());
    boost::archive::text_iarchive ia(iss);
    std::string message;

    try {
        ia >> message;
    } catch (const boost::archive::archive_exception& e) {
        std::cerr << "Erreur d'archive Boost: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur générique: " << e.what() << std::endl;
    } 

    // non serialized message
    // std::string message(receive_buffer.begin(), receive_buffer.begin() + bytes_transferred);
    // std::cout << "Received: '" << message << "'" << std::endl;

    if (message.rfind("$PING", 0) == 0) {
        sendTCP("PONG");
    } else if (message.rfind("$121", 0) == 0) {
        size_t space_pos = message.find(' ', 4);
        if (space_pos != std::string::npos) {
            std::string response = message.substr(space_pos + 1, 2);
            std::string username = message.substr(space_pos + 4);

            std::cout << "Response: " << response << ", Username: " << username << std::endl;

            if (response == "OK") {
                if (loginCallback) {
                    loginCallback(true);
                }
            } else if (response == "KO") {
                if (loginCallback) {
                    loginCallback(false);
                }
            }
        } else {
            std::cerr << "Invalid response format for command 121" << std::endl;
        }
    } else if (message.rfind("$136", 0) == 0) {
        std::cout << "Received rooms" << std::endl;
        size_t space_pos = message.find(' ', 4);
        if (space_pos != std::string::npos) {
            std::string rooms = message.substr(space_pos + 1);
            // std::cout << "Rooms: " << rooms << std::endl;
            std::vector<Room> room_list = stockRooms(rooms);
            if (roomsCallback) {
                roomsCallback(room_list);
            }
        }
    } else if (message.rfind("$124", 0) == 0) {
        size_t space_pos = message.find(' ', 4);
        if (space_pos != std::string::npos) {
            std::string msg = message.substr(space_pos + 1);
            std::cout << "Message: " << msg << std::endl;
            if (messageCallback) {
                messageCallback(msg);
            }
        }
    } else if (message.rfind("$131", 0) == 0) {
        size_t space_pos = message.find(' ', 4);
        if (space_pos != std::string::npos) {
            std::string usernames = message.substr(space_pos + 1);
            // std::cout << "Usernames: " << usernames << std::endl;
            std::vector<std::string> username_list;
            std::istringstream ss(usernames);
            std::string username;
            while (std::getline(ss, username, ',')) {
                username_list.push_back(username);
            }
        }
    } else if (message.rfind("$132", 0) == 0) {
        size_t space_pos = message.find(' ', 4);
        if (space_pos != std::string::npos) {
            std::string stats = message.substr(space_pos + 1);
            std::vector<Statistic> stats_list = stockPlayerStatistics(stats);
            if (playerStatInRoomCallback) {
                playerStatInRoomCallback(stats_list);
            }
            // for (const auto& stat : stats_list) {
            //     std::cout << "Player ID: " << stat.player_id << std::endl;
            //     std::cout << "Number of Deaths: " << stat.nbrDeaths << std::endl;
            //     std::cout << "Number of Games: " << stat.nbrGames << std::endl;
            //     std::cout << "Number of Kills: " << stat.nbrKills << std::endl;
            //     std::cout << "Number of Losses: " << stat.nbrLosses << std::endl;
            //     std::cout << "Number of Wins: " << stat.nbrWins << std::endl;
            //     std::cout << "Score: " << stat.score << std::endl;
            // }
        }
    } else {
        std::cerr << "Unknown command received: " << message << std::endl;
    }
    startReceiving();
}

void Network::setLoginCallback(std::function<void(bool)> callback)
{
    this->loginCallback = callback;
}

void Network::setRoomsCallback(std::function<void(const std::vector<Room>&)> callback)
{
    this->roomsCallback = callback;
}

void Network::setMessageCallback(std::function<void(const std::string&)> callback)
{
    this->messageCallback = callback;
}

void Network::sendLogin(const std::string& username, const std::string& password) {
    if (connected) {
        sendTCP("121 " + username + " " + password);
    }
}

void Network::sendGetRooms() {
    if (connected) {
        sendTCP("136");
    }
}

std::vector<Room> Network::stockRooms(const std::string& rooms) {
    std::vector<Room> room_list;

    try {
        auto j = json::parse(rooms);

        for (auto& [key, value] : j.items()) {
            Room room;
            room.id = key;
            room.room_name = value["room name"];
            room.room_password = value["room password"];
            room.room_state = value["room state"];
            room.max_players = std::stoi(value["max players"].get<std::string>());
            room.current_players = std::stoi(value["current players"].get<std::string>());
            room_list.push_back(room);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error parsing rooms JSON: " << e.what() << std::endl;
    }

    return room_list;
}

std::vector<Statistic> Network::stockPlayerStatistics(const std::string& statistics) {
    std::vector<Statistic> stats_list;
    std::istringstream iss(statistics);
    std::string stat;
    std::string jsonString;

    while (std::getline(iss, stat, '}')) {
        if (stat.empty()) continue;
        stat += '}';
        jsonString += stat;
        try {
            auto j = json::parse(jsonString);
            Statistic stats;
            stats.player_id = j["id"];
            stats.nbrDeaths = j["nbrDeaths"];
            stats.nbrGames = j["nbrGames"];
            stats.nbrKills = j["nbrKills"];
            stats.nbrLosses = j["nbrLosses"];
            stats.nbrWins = j["nbrWins"];
            stats.score = j["score"];
            stats_list.push_back(stats);
            jsonString.clear();
        } catch (const json::parse_error& e) {
            if (e.id == 101) {
                continue;
            } else {
                std::cerr << "Error parsing player statistics JSON: " << e.what() << std::endl;
                jsonString.clear();
            }
        }
    }

    return stats_list;
}

void Network::sendGetPlayerInRoom() {
    if (connected) {
        sendTCP("119");
    }
}

void Network::sendGetPlayerStatInRoom(const std::string& room_id) {
    if (connected) {
        sendTCP("132 " + room_id);
    }
}

void Network::sendKickPlayer(const std::string &id) {
    if (connected) {
        sendTCP("133 " + id);
    }
}

void Network::sendBanPlayer(const std::string &id) {
    if (connected) {
        sendTCP("134 " + id);
    }
}

void Network::setPlayerStatInRoomCallback(std::function<void(const std::vector<Statistic> &)> callback)
{
    this->playerStatInRoomCallback = callback;
}