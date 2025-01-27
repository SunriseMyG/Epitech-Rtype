#include "Protocol.hpp"

Protocol::Protocol(boost::asio::io_context &io_context, short port, bool disable_serialization)
    : udp_socket_(io_context, udp::endpoint(udp::v4(), port)),
      tcp_acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
      next_player_id_(1), disable_serialization_(disable_serialization)
{
    std::cout << "Initialisation du protocole sur le port " << port << std::endl;

    tcp_command_handlers_ = {
        {"PONG", std::bind(&Protocol::handleCommandPong, this, std::placeholders::_1, std::placeholders::_2)},
        {"100", std::bind(&Protocol::handleCommand100, this, std::placeholders::_1, std::placeholders::_2)},
        {"102", std::bind(&Protocol::handleCommand102, this, std::placeholders::_1, std::placeholders::_2)},
        {"103", std::bind(&Protocol::handleCommand103, this, std::placeholders::_1, std::placeholders::_2)},
        {"104", std::bind(&Protocol::handleCommand104, this, std::placeholders::_1, std::placeholders::_2)},
        {"105", std::bind(&Protocol::handleCommand105, this, std::placeholders::_1, std::placeholders::_2)},
        {"112", std::bind(&Protocol::handleCommand112, this, std::placeholders::_1, std::placeholders::_2)},
        {"113", std::bind(&Protocol::handleCommand113, this, std::placeholders::_1, std::placeholders::_2)},
        {"118", std::bind(&Protocol::handleCommand118, this, std::placeholders::_1, std::placeholders::_2)},
        {"119", std::bind(&Protocol::handleCommand119, this, std::placeholders::_1, std::placeholders::_2)},
        {"120", std::bind(&Protocol::handleCommand120, this, std::placeholders::_1, std::placeholders::_2)},
        {"121", std::bind(&Protocol::handleCommand121, this, std::placeholders::_1, std::placeholders::_2)},
        {"122", std::bind(&Protocol::handleCommand122, this, std::placeholders::_1, std::placeholders::_2)},
        {"123", std::bind(&Protocol::handleCommand123, this, std::placeholders::_1, std::placeholders::_2)},
        {"124", std::bind(&Protocol::handleCommand124, this, std::placeholders::_1, std::placeholders::_2)},
        {"125", std::bind(&Protocol::handleCommand125, this, std::placeholders::_1, std::placeholders::_2)},
        {"126", std::bind(&Protocol::handleCommand126, this, std::placeholders::_1, std::placeholders::_2)},
        {"127", std::bind(&Protocol::handleCommand127, this, std::placeholders::_1, std::placeholders::_2)},
        {"129", std::bind(&Protocol::handleCommand129, this, std::placeholders::_1, std::placeholders::_2)},
        {"130", std::bind(&Protocol::handleCommand130, this, std::placeholders::_1, std::placeholders::_2)},
        {"131", std::bind(&Protocol::handleCommand131, this, std::placeholders::_1, std::placeholders::_2)},
        {"132", std::bind(&Protocol::handleCommand132, this, std::placeholders::_1, std::placeholders::_2)},
        {"133", std::bind(&Protocol::handleCommand133, this, std::placeholders::_1, std::placeholders::_2)},
        {"134", std::bind(&Protocol::handleCommand134, this, std::placeholders::_1, std::placeholders::_2)},
        {"135", std::bind(&Protocol::handleCommand135, this, std::placeholders::_1, std::placeholders::_2)},
        {"136", std::bind(&Protocol::handleCommand136, this, std::placeholders::_1, std::placeholders::_2)}
    };

    udp_command_handlers_ = {
        {"dump", std::bind(&Protocol::dumpUDP, this, std::placeholders::_1, std::placeholders::_2)},
        {"101", std::bind(&Protocol::handleCommand101, this, std::placeholders::_1, std::placeholders::_2)},
        {"106", std::bind(&Protocol::handleCommand106, this, std::placeholders::_1, std::placeholders::_2)},
        {"107", std::bind(&Protocol::handleCommand107, this, std::placeholders::_1, std::placeholders::_2)},
        {"108", std::bind(&Protocol::handleCommand108, this, std::placeholders::_1, std::placeholders::_2)},
        {"109", std::bind(&Protocol::handleCommand109, this, std::placeholders::_1, std::placeholders::_2)},
        {"110", std::bind(&Protocol::handleCommand110, this, std::placeholders::_1, std::placeholders::_2)},
        {"111", std::bind(&Protocol::handleCommand111, this, std::placeholders::_1, std::placeholders::_2)},
        {"115", std::bind(&Protocol::handleCommand115, this, std::placeholders::_1, std::placeholders::_2)},
        {"116", std::bind(&Protocol::handleCommand116, this, std::placeholders::_1, std::placeholders::_2)}
    };


    database_.checkAndCreateDatabaseFile("server/Database/table/users.json");
    database_.checkAndCreateDatabaseFile("server/Database/table/leaderboards.json");

    game_engine_.setFunctionsendEntitiesClients(std::function<void(std::shared_ptr<Room>)>([this](std::shared_ptr<Room> room) {
        std::string entitiesData = getEntitiesInfoAsJson(room);

        if (entitiesData.empty() || entitiesData == "{}") {
            return;
        }
    
        for (const auto& player : room->getPlayers()) {
            auto player_udp_endpoint = player->getUDPRemote();
            if (player_udp_endpoint) {
                sendUDPMessage("$115 " + entitiesData + "\n", *player_udp_endpoint);
            } else {
                std::cerr << "$115 KO: Invalid player or player UDP endpoint" << std::endl;
            }
        }
    }));

    game_engine_.setFunctionsendLevelClients(std::function<void(std::shared_ptr<Room>)>([this](std::shared_ptr<Room> room) {
        std::string level = std::to_string(room->getLevel());
    
        for (const auto& player : room->getPlayers()) {
            auto player_udp_endpoint = player->getUDPRemote();
            if (player_udp_endpoint) {
                sendUDPMessage("$109 " + level + "\n", *player_udp_endpoint);
            } else {
                std::cerr << "$109 KO: Invalid player or player UDP endpoint" << std::endl;
            }
        }
    }));

    game_engine_.setFunctionsendEndGameClients(std::function<void(std::shared_ptr<Room>, bool)>([this](std::shared_ptr<Room> room, bool win) {
        std::string result = win ? "WIN" : "LOSE";

        for (const auto& player : room->getPlayers()) {
            auto player_tcp_socket = player->getTCPSocket();
            if (player_tcp_socket) {
                sendTCPMessage("$114 " + result + "\n", player_tcp_socket);
            } else {
                std::cerr << "$114 KO: Invalid player or player TCP socket" << std::endl;
            }
        }
    }));

    game_engine_.setFunctionsendKickPlayer(std::function<void(int, std::string)>([this](int playerId, std::string reason) {
        auto player = players_by_id_[playerId];
        auto player_udp_endpoint = player->getUDPRemote();
        if (player_udp_endpoint) {
            sendUDPMessage("$128 " + reason + "\n", *player_udp_endpoint);
        } else {
            std::cerr << "$128 KO: Invalid player or player UDP endpoint" << std::endl;
        }
    }));

    pingLoop();
}

std::string Protocol::prepareMessage(const std::string &message) {
    if (this->disable_serialization_) {
        return message;
    }
    std::ostringstream oss;
    boost::archive::text_oarchive oa(oss);
    oa << message;
    return oss.str();
}

std::vector<std::string> Protocol::parseMessage(const std::string &message)
{
    std::istringstream iss(message);
    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg)
        args.push_back(arg);
    return args;
}

void Protocol::handleCommand100(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 100" << std::endl;
    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());
    int player_id = players_[player_key]->getId();
    sendTCPMessage("$100 " + std::to_string(player_id) + "\n", socket_ptr);
}

void Protocol::handleCommand101(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 101" << std::endl;
    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$101 KO: Invalid command format", *remote_endpoint);
        return;
    }

    int player_id;
    try {
        player_id = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        sendUDPMessage("$101 KO: Invalid player ID format", *remote_endpoint);
        return;
    } catch (const std::out_of_range &e) {
        sendUDPMessage("$101 KO: Player ID out of range", *remote_endpoint);
        return;
    }

    if (players_by_id_.find(player_id) == players_by_id_.end()) {
        sendUDPMessage("$101 KO: Unknown player ID", *remote_endpoint);
        return;
    }

    auto player = players_by_id_[player_id];
    player->setUDPPort(remote_endpoint->port());
    player->setUDPRemote(remote_endpoint);
    std::cout << "Player " << player_id << " registered with UDP port " << remote_endpoint->port() << std::endl;
    sendUDPMessage("$101 OK\n", *remote_endpoint);
}

void Protocol::handleCommand102(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 102" << std::endl;
    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 3 || args.size() > 4) {
        sendTCPMessage("$102 KO: Invalid parameter\n", socket_ptr);
        return;
    }

    std::string room_name = args[1];
    std::string room_password = "";
    int max_players;

    if (args.size() == 3) {
        try {
            max_players = std::stoi(args[2]);
        } catch (const std::invalid_argument &e) {
            sendTCPMessage("$102 KO: Invalid max players format\n", socket_ptr);
            return;
        } catch (const std::out_of_range &e) {
            sendTCPMessage("$102 KO: Max players out of range\n", socket_ptr);
            return;
        }
    } else {
        room_password = args[2];
        try {
            max_players = std::stoi(args[3]);
        } catch (const std::invalid_argument &e) {
            sendTCPMessage("$102 KO: Invalid max players format\n", socket_ptr);
            return;
        } catch (const std::out_of_range &e) {
            sendTCPMessage("$102 KO: Max players out of range\n", socket_ptr);
            return;
        }
    }

    int room_id = room_manager_.addRoomIncrement();
    auto room = std::make_shared<Room>(room_id, room_name, room_password, Room::State::WAITING, max_players, true);
    room_manager_.addRoom(room);
    room_manager_.dumpRooms();

    sendTCPMessage("$102 OK\n", socket_ptr);
}

void Protocol::handleCommand103(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 103" << std::endl;
    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2 || args.size() > 3) {
        sendTCPMessage("$103 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int room_id;
    try {
        room_id = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        sendTCPMessage("$103 KO: Invalid room ID format\n", socket_ptr);
        return;
    } catch (const std::out_of_range &e) {
        sendTCPMessage("$103 KO: Room ID out of range\n", socket_ptr);
        return;
    }

    std::string room_password = args.size() == 3 ? args[2] : "";

    auto room = room_manager_.getRoom(room_id);
    if (room == nullptr) {
        sendTCPMessage("$103 KO: Room not found\n", socket_ptr);
        return;
    }

    if (room->getState() != IRoom::State::WAITING) {
        sendTCPMessage("$103 KO: Room not waiting\n", socket_ptr);
        return;
    }

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());
    auto player = players_[player_key];

    for (const auto& r : room_manager_.getRooms()) {
        for (const auto& p : r->getPlayers()) {
            if (p->getId() == player->getId()) {
                sendTCPMessage("$103 KO: Player already in a room\n", socket_ptr);
                return;
            }
        }
    }

    for (const auto& p : room->getPlayers()) {
        if (p->getId() == player->getId()) {
            sendTCPMessage("$103 KO: Player already in the room\n", socket_ptr);
            return;
        }
    }

    if (!room->getPassword().empty() && room->getPassword() != room_password) {
        sendTCPMessage("$103 KO: Incorrect password\n", socket_ptr);
        return;
    }

    room_manager_.playerJoinRoom(players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())], room_id);
    // room_manager_.dumpRooms();
    sendTCPMessage("$103 OK\n", socket_ptr);
}

void Protocol::handleCommand104(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 104" << std::endl;
    std::vector<std::shared_ptr<Room>> rooms = room_manager_.getRooms();
    std::string json_response = "{";

    for (const auto& room : rooms) {
        if (room->getState() != IRoom::State::WAITING || room->getPlayers().size() == room->getMaxPlayers() || !room->isPrivate()) {
            continue;
        }
        json_response += "\"" + std::to_string(room->getId()) + "\": {";
        json_response += "\"room name\": \"" + room->getName() + "\", ";
        json_response += "\"room password\": \"" + room->getPassword() + "\", ";
        json_response += "\"room state\": \"" + std::to_string(static_cast<int>(room->getState())) + "\", ";
        json_response += "\"max players\": \"" + std::to_string(room->getMaxPlayers()) + "\", ";
        json_response += "\"current players\": \"" + std::to_string(room->getPlayers().size()) + "\"";
        json_response += "}, ";
    }

    if (!rooms.empty()) {
        json_response.pop_back();
        json_response.pop_back();
    }

    json_response += "}\n";

    sendTCPMessage("$104 " + json_response, socket_ptr);
}

void Protocol::handleCommand136(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 136" << std::endl;
    std::vector<std::shared_ptr<Room>> rooms = room_manager_.getRooms();
    std::string json_response = "{";

    for (const auto& room : rooms) {
        json_response += "\"" + std::to_string(room->getId()) + "\": {";
        json_response += "\"room name\": \"" + room->getName() + "\", ";
        json_response += "\"room password\": \"" + room->getPassword() + "\", ";
        json_response += "\"room state\": \"" + std::to_string(static_cast<int>(room->getState())) + "\", ";
        json_response += "\"max players\": \"" + std::to_string(room->getMaxPlayers()) + "\", ";
        json_response += "\"current players\": \"" + std::to_string(room->getPlayers().size()) + "\"";
        json_response += "}, ";
    }

    if (!rooms.empty()) {
        json_response.pop_back();
        json_response.pop_back();
    }

    json_response += "}\n";

    sendTCPMessage("$136 " + json_response, socket_ptr);
}

void Protocol::handleCommand105(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 105" << std::endl;

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getTCPPort() == socket_ptr->remote_endpoint().port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendTCPMessage("$105 KO: Player not in any room", socket_ptr);
        return;
    }

    if (player_room->getState() == IRoom::State::PLAYING) {
        sendTCPMessage("$105 KO: Party is started\n", socket_ptr);
        return;
    }

    // player_room->setDifficulty(IRoom::Difficulty::HARD);
    // print room difficulty
    std::cout << "Room difficulty: " << static_cast<int>(player_room->getDifficulty()) << std::endl;
    player_room->setState(IRoom::State::PLAYING);
    // room_manager_.dumpRooms();

    game_engine_.run(player_room);

    for (const auto& player : player_room->getPlayers()) {
        if (player && player->getTCPSocket()) {
            sendTCPMessage("$105 OK\n", player->getTCPSocket());
        } else {
            std::cerr << "$105 KO: Invalid player or player socket" << std::endl;
        }
    }
}

void Protocol::handleCommand106(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 106" << std::endl;

    std::cout << "Client UDP port: " << remote_endpoint->port() << std::endl;

    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$106 KO: Invalid command format", *remote_endpoint);
        return;
    }

    std::string key_pressed = args[1];
    int sender_id = -1;
    std::shared_ptr<Room> player_room = nullptr;

    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                sender_id = player->getId();
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendUDPMessage("$106 KO: Player not in any room", *remote_endpoint);
        return;
    }

    std::string update_message = "$106 OK " + key_pressed + " " + std::to_string(sender_id) + "\n";
    for (const auto& player : player_room->getPlayers()) {
        if (player->getUDPPort() != remote_endpoint->port()) {
            sendUDPMessage(update_message, *player->getUDPRemote());
        }
    }
    sendUDPMessage("$106 OK\n", *remote_endpoint);
}

void Protocol::handleCommand107(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 107" << std::endl;
    std::cout << "Client UDP port: " << remote_endpoint->port() << std::endl;

    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$107 KO: Invalid command format", *remote_endpoint);
        return;
    }

    std::string key_pressed = args[1];
    int sender_id = -1;
    std::shared_ptr<Room> player_room = nullptr;

    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                sender_id = player->getId();
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr || sender_id == -1) {
        sendUDPMessage("$107 KO: Player not in any room", *remote_endpoint);
        return;
    }

    auto new_position = game_engine_.playerMoove(player_room, sender_id, key_pressed);

    std::string update_message = "$107 OK " + key_pressed + " " + std::to_string(sender_id) + " " + std::to_string(new_position.first) + " " + std::to_string(new_position.second) + "\n";
    for (const auto& player : player_room->getPlayers()) {
        // if (player->getUDPPort() != remote_endpoint->port()) {
        sendUDPMessage(update_message, *player->getUDPRemote());
        // }
    }
    sendUDPMessage("$107 OK\n", *remote_endpoint);
}

void Protocol::handleCommand108(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 108" << std::endl;

    std::cout << "Client UDP port: " << remote_endpoint->port() << std::endl;

    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$108 KO: Invalid command format", *remote_endpoint);
        return;
    }

    int scoreReceived = atoi(args[1].c_str()) || 0;

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendUDPMessage("$108 KO: Player not in any room", *remote_endpoint);
        return;
    }

    int newscore = player_room->getScore() + scoreReceived;

    player_room->setScore(newscore);

    for (const auto& player : player_room->getPlayers()) {
        sendUDPMessage("$108 OK " + std::to_string(newscore) + "\n", *player->getUDPRemote());
    }
}

void Protocol::handleCommand109(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 109" << std::endl;

    std::cout << "Client UDP port: " << remote_endpoint->port() << std::endl;

    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$109 KO: Invalid command format", *remote_endpoint);
        return;
    }

    int level = atoi(args[1].c_str());

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendUDPMessage("$109 KO: Player not in any room", *remote_endpoint);
        return;
    }

    player_room->setLevel(level);

    for (const auto& player : player_room->getPlayers()) {
        sendUDPMessage("$109 " +  std::to_string(level) + "\n", *player->getUDPRemote());
    }
}


void Protocol::handleCommand110(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 110" << std::endl;

    std::cout << "Client UDP port: " << remote_endpoint->port() << std::endl;

    std::vector<std::string> args = parseMessage(message);
    if (args.size() < 2) {
        sendUDPMessage("$110 KO: Invalid command format", *remote_endpoint);
        return;
    }

    int level = atoi(args[1].c_str());

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendUDPMessage("$110 KO: Player not in any room", *remote_endpoint);
        return;
    }

    player_room->setLevel(level);

    for (const auto& player : player_room->getPlayers()) {
        if (player->getUDPPort() != remote_endpoint->port()) {
            sendUDPMessage("$110 " + std::to_string(player->getId()) + "\n", *player->getUDPRemote());
        }
    }
}

void Protocol::handleCommand111(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 111" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() < 2) {
        sendUDPMessage("$111 KO: Invalid command format", *remote_endpoint);
        return;
    }

    int mob_id = atoi(args[1].c_str());

    std::shared_ptr<Room> player_room = nullptr;

    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendUDPMessage("$111 KO: Player not in any room", *remote_endpoint);
        return;
    }

    std::string update_message = "$111 " + std::to_string(mob_id);

    for (const auto& player : player_room->getPlayers()) {
        if (player->getUDPPort() != remote_endpoint->port()) {
            sendUDPMessage(update_message + "\n", *player->getUDPRemote());
        }
    }

    sendUDPMessage("$111 OK\n", *remote_endpoint);
}

void Protocol::handleCommand112(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 112" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    if (players_.find(player_key) == players_.end()) {
        sendTCPMessage("$112 KO: Player not found", socket_ptr);
        return;
    }

    int player_id = players_[player_key]->getId();

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendTCPMessage("$112 KO: Player not in any room", socket_ptr);
        return;
    }

    room_manager_.playerLeaveRoom(player_id);
    // room_manager_.dumpRooms();

    if (player_room->getPlayers().empty()) {
        room_manager_.removeRoom(player_room);
        std::cout << "Room " << player_room->getId() << " is empty and has been removed." << std::endl;
    }

    sendTCPMessage("$112 OK\n", socket_ptr);

    std::string remaining_players_info;
    for (const auto& player : player_room->getPlayers()) {
        remaining_players_info += std::to_string(player->getId()) + " ";
    }
    if (!remaining_players_info.empty()) {
        remaining_players_info.pop_back();
    }

    for (const auto& player : player_room->getPlayers()) {
        auto player_socket = player->getTCPSocket();
        if (player_socket && player_socket != socket_ptr) {
            sendTCPMessage("$112 " + remaining_players_info + "\n", player_socket);
        }
    }
}

void Protocol::handleCommand113(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 113" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    if (players_.find(player_key) == players_.end()) {
        sendTCPMessage("$113 KO: Player not found", socket_ptr);
        return;
    }

    int player_id = players_[player_key]->getId();

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendTCPMessage("$113 KO: Player not in any room", socket_ptr);
        return;
    }

    for (const auto& player : player_room->getPlayers()) {
        if (player && player->getTCPSocket()) {
            sendTCPMessage("$113 " + std::to_string(player_id) + "\n", player->getTCPSocket());
        } else {
            std::cerr << "$113 KO: Invalid player or player socket" << std::endl;
        }
    }

    room_manager_.removeRoom(player_room);
    room_manager_.dumpRooms();
    sendTCPMessage("$113 OK\n", socket_ptr);
}

void Protocol::handleCommand115(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 115" << std::endl;

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getUDPPort() == remote_endpoint->port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        std::cerr << "Player not in any room" << std::endl;
        sendUDPMessage("$115 KO: Player not in any room", *remote_endpoint);
        return;
    }

    std::string enemy_positions = getEntitiesInfoAsJson(player_room);

    for (const auto& player : player_room->getPlayers()) {
        auto player_udp_endpoint = player->getUDPRemote();
        if (player_udp_endpoint) {
            sendUDPMessage("$115 " + enemy_positions + "\n", *player_udp_endpoint);
        } else {
            std::cerr << "$115 KO: Invalid player or player UDP endpoint" << std::endl;
        }
    }
}

std::string Protocol::getEntitiesInfoAsJson(std::shared_ptr<Room> room)
{
    std::ostringstream json;
    json << "{";
    bool first = true;

    for (const auto& entity : room->getEntities()) {
        auto state = entity->getComponent<StateComponent>()->state;

        if ((state == "NONE")) {
            continue;
        }

        auto position = entity->getComponent<PositionComponent>();
        auto group = entity->getGroup();
        auto type = entity->getType();
        auto health = entity->getComponent<HealthComponent>();
        auto score = entity->getComponent<ScoreComponent>();
        auto velocity = entity->getComponent<VelocityComponent>();

        if (!first) {
            json << ", ";
        }
        first = false;
        json << "\"" << entity->getId() << "\": {";
        
        if (state == "DELETE")
            json << "\"state\": \"" << state << "\"";
        else 
            json << "\"state\": \"" << state << "\",";

        if (state == "CREATE") {
            json << "\"group\": \"" << group << "\",";
            json << "\"type\": \"" << type << "\",";
            if (group == "PLAYER") {
                int playerId = room->getPlayerIdFromEntity(entity);
                json << "\"ID NETWORK\": " << playerId << ",";
            }
        }

        if (state == "UPDATE" || state == "CREATE") {
            json << "\"x\": " << std::fixed << position->x << ",";
            json << "\"y\": " << std::fixed << position->y << ",";
            json << "\"vx\": " << std::fixed << velocity->x << ",";
            json << "\"vy\": " << std::fixed << velocity->y << ",";
            if (health)
                json << "\"health\": " << health->health << ",";
            else
                json << "\"health\": 0,";
            if (score)
                json << "\"score\": " << score->score;
            else
                json << "\"score\": 0";

        }
        json << "}";

        entity->getComponent<StateComponent>()->state = "NONE";
    }
    json << "}";
    return json.str();
}

void Protocol::handleCommand116(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Handling command 116" << std::endl;
}

void Protocol::handleCommand118(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 118" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 2) {
        sendTCPMessage("$120 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string difficulty = args[1];

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getTCPPort() == socket_ptr->remote_endpoint().port()) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendTCPMessage("$118 KO: Player not in any room\n", socket_ptr);
        return;
    }

    if (difficulty == "EASY") {
        player_room->setDifficulty(IRoom::Difficulty::EASY);
        std::cout << "Room difficulty: " << player_room->getDifficulty() << std::endl;
    } else if (difficulty == "MEDIUM") {
        player_room->setDifficulty(IRoom::Difficulty::MEDIUM);
        std::cout << "Room difficulty: " << player_room->getDifficulty() << std::endl;
    } else if (difficulty == "HARD") {
        player_room->setDifficulty(IRoom::Difficulty::HARD);
        std::cout << "Room difficulty: " << player_room->getDifficulty() << std::endl;
    } else {
        sendTCPMessage("$118 KO: Invalid difficulty\n", socket_ptr);
        return;
    }

    for (const auto& player : player_room->getPlayers()) {
        if (player && player->getTCPSocket()) {
            sendTCPMessage("$118 OK\n", player->getTCPSocket());
        } else {
            std::cerr << "$118 KO: Invalid player or player socket\n" << std::endl;
        }
    }
}

void Protocol::handleCommand119(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 119" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    if (players_.find(player_key) == players_.end()) {
        sendTCPMessage("$119 KO: Player not found\n", socket_ptr);
        return;
    }

    int player_id = players_[player_key]->getId();

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr) {
        sendTCPMessage("$119 KO: Player not in any room\n", socket_ptr);
        return;
    }

    std::string response;
    for (const auto& player : player_room->getPlayers()) {
        response += std::to_string(player->getId()) + " ";
    }

    if (!response.empty()) {
        response.pop_back();
    }

    for (const auto& player : player_room->getPlayers()) {
        if (player && player->getTCPSocket()) {
            sendTCPMessage("$119 " + response + "\n", player->getTCPSocket());
        }
    }
}

void Protocol::handleCommand120(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    // register
    std::cout << "Handling command 120" << std::endl;

    std::vector<std::string> args = parseMessage(message);
    
    if (args.size() < 3) {
        sendTCPMessage("$120 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string username = args[1];
    std::string password = args[2];

    if (database_.checkIfUsernameAlreadyTaken(username) == true) {
        sendTCPMessage("$120 KO: Username already taken\n", socket_ptr);
        std::cout << "Username already taken" << std::endl;
        return;
    }

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    database_.setUserId(players_[player_key]->getId());
    database_.setUsernameFromId(players_[player_key]->getId(), username);
    database_.setPasswordFromId(players_[player_key]->getId(), password);

    database_.setAdminFromId(players_[player_key]->getId(), false);
    database_.setBanFromId(players_[player_key]->getId(), false);
    database_.setStatsFromId("server/Database/table/statistic.json", players_[player_key]->getId(), 0, 0, 0, 0, 0, 0);
    sendTCPMessage("$120 OK " + std::to_string(players_[player_key]->getId()) + " " + username + "\n", socket_ptr);
}

void Protocol::handleCommand121(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    // login
    std::cout << "Handling command 121" << std::endl;

    std::vector<std::string> args = parseMessage(message);
    
    if (args.size() < 3) {
        sendTCPMessage("$121 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string username = args[1];
    std::string password = args[2];

    // if player exist and the password is correct we set the database information to the player
    if (database_.checkIfPlayerAlreadyExist(username, password) == true) {
        players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())]->setId(database_.getIdFromUsernameAndPassword(username, password));
        players_by_id_[players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())]->getId()] = players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())];
        std::cout << "Player already exist" << std::endl;
    } else {
        sendTCPMessage("$121 KO: Player doesn't exist\n", socket_ptr);
        std::cout << "Player doesn't exist" << std::endl;
        return;
    }

    if (database_.checkIfPlayerIsBanned(username)) {
        sendTCPMessage("$121 KO: Player is banned\n", socket_ptr);
        std::cout << "Player is banned" << std::endl;
        return;
    }

    sendTCPMessage("$121 OK " + std::to_string(players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())]->getId()) + " " + username + "\n", socket_ptr);
}

void Protocol::handleCommand122(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    // send private message
    std::cout << "Handling command 122" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() < 3) {
        sendTCPMessage("$122 KO: Invalid command format\n", socket_ptr);
        return;
    }

    if (this->banWordCheck(122, message, socket_ptr))
        return;

    std::string username = args[1];
    std::string private_message;
    for (size_t i = 2; i < args.size(); ++i) {
        private_message += args[i] + " ";
    }
    if (!private_message.empty()) {
        private_message.pop_back();
    }

    std::cout << "Recipient username: " << username << std::endl;

    int recipient_id = database_.getIdFromUsername(username);

    std::cout << "Recipient ID: " << recipient_id << std::endl;

    if (recipient_id == -1) {
        sendTCPMessage("$122 KO: Recipient not found\n", socket_ptr);
        return;
    }

    std::shared_ptr<Player> recipient = nullptr;
    for (const auto& player : players_) {
        if (player.second->getId() == recipient_id) {
            recipient = player.second;
            break;
        }
    }

    if (recipient == nullptr) {
        sendTCPMessage("$122 KO: Recipient not connected\n", socket_ptr);
        return;
    }


    std::string sender_username = database_.getUsernameFromId(players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())]->getId());

    sendTCPMessage("$122 " + sender_username + ": " + private_message + "\n", socket_ptr);
    sendTCPMessage("$122 " + sender_username + ": " + private_message + "\n", recipient->getTCPSocket());
}

void Protocol::handleCommand123(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    // send public message
    std::cout << "Handling command 123" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    std::vector<std::string> args = parseMessage(message);

    bool isInRoom = false;

    if (args.size() < 2) {
        sendTCPMessage("$123 KO: Invalid command format\n", socket_ptr);
        return;
    }

    if (this->banWordCheck(123, message, socket_ptr))
        return;

    int player_id = players_[player_key]->getId();

    std::shared_ptr<Room> player_room = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                isInRoom = true;
                player_room = room;
                break;
            }
        }
        if (isInRoom) {
            break;
        }
    }

    if (!isInRoom) {
        sendTCPMessage("$123 KO: You are not in a room. Use command 124 instead.\n", socket_ptr);
        return;
    }

    std::string public_message;
    for (size_t i = 1; i < args.size(); ++i) {
        public_message += args[i] + " ";
    }
    if (!public_message.empty()) {
        public_message.pop_back();
    }

    // sendTCPMessage("$123 OK\n", socket_ptr);

    std::string sender_username = database_.getUsernameFromId(players_[socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port())]->getId());

    for (const auto& player : player_room->getPlayers()) {
        if (player->getId() != player_id) {
            sendTCPMessage("$123 " + sender_username + ": " + public_message + "\n", player->getTCPSocket());
        }
    }
}

void Protocol::handleCommand124(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    // send message to all players in the main menu
    std::cout << "Handling command 124" << std::endl;

    if (this->banWordCheck(124, message, socket_ptr))
        return;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());
    int player_id = players_[player_key]->getId();

    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                sendTCPMessage("$124 KO: You are in a room. Use command 123 instead.\n", socket_ptr);
                return;
            }
        }
    }

    std::vector<std::string> args = parseMessage(message);

    if (args.size() < 2) {
        sendTCPMessage("$124 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string public_message;
    for (size_t i = 1; i < args.size(); ++i) {
        public_message += args[i] + " ";
    }
    if (!public_message.empty()) {
        public_message.pop_back();
    }

    std::cout << "Public message: " << public_message << std::endl;

    // sendTCPMessage("$124 OK\n", socket_ptr);

    std::string sender_username = database_.getUsernameFromId(player_id);

    std::string full_message = "$124 " + sender_username + ": " + public_message + "\n";

    for (const auto& player_pair : players_) {
        auto player = player_pair.second;
        bool isInRoom = false;

        for (const auto& room : room_manager_.getRooms()) {
            for (const auto& room_player : room->getPlayers()) {
                if (room_player->getId() == player->getId()) {
                    isInRoom = true;
                    break;
                }
            }
            if (isInRoom) {
                break;
            }
        }

        if (!isInRoom) {
            sendTCPMessage(full_message, player->getTCPSocket());
        }
    }
}

void Protocol::handleCommand125(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 125" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() > 1) {
        sendTCPMessage("$125 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string leaderboard = database_.getLeaderboard("server/Database/table/leaderboards.json");
    
    // Supprimer les \n et les espaces superflus
    leaderboard.erase(std::remove(leaderboard.begin(), leaderboard.end(), '\n'), leaderboard.end());
    
    // Formater le JSON de manière plus compacte
    json j = json::parse(leaderboard);
    std::string formattedLeaderboard = j.dump();

    sendTCPMessage("$125 " + formattedLeaderboard + "\n", socket_ptr);
}

void Protocol::handleCommand126(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 126" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() < 2) {
        sendTCPMessage("$126 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int player_id;
    try {
        player_id = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        sendTCPMessage("$126 KO: Invalid player ID format\n", socket_ptr);
        return;
    }

    int score;
    try {
        score = std::stoi(args[2]);
    } catch (const std::invalid_argument &e) {
        sendTCPMessage("$126 KO: Invalid score format\n", socket_ptr);
        return;
    }

    std::cout << "Player ID: " << player_id << std::endl;
    std::string player_name = database_.getUsernameFromId(player_id);
    std::cout << "Player name: " << player_name << std::endl;

    if (player_name.empty()) {
        sendTCPMessage("$126 KO: Player not found\n", socket_ptr);
        return;
    }

    int position = database_.setNewScoreInLeaderBoard("server/Database/table/leaderboards.json", player_name, score);

    if (position == -1) {
        sendTCPMessage("$126 KO: Failed to update leaderboard\n", socket_ptr);
    } else {
        sendTCPMessage("$126 OK " + std::to_string(position) + "\n", socket_ptr);
    }
}

void Protocol::handleCommand127(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 127" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    if (players_.find(player_key) == players_.end()) {
        sendTCPMessage("$127 KO: Player not found\n", socket_ptr);
        return;
    }

    room_manager_.matchmaking(players_[player_key]);
    sendTCPMessage("$127 OK\n", socket_ptr);
}

void Protocol::handleCommand129(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 129" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    std::string contentStatistics = database_.getStatistics("server/Database/table/statistic.json");

    sendTCPMessage("$129 " + contentStatistics + "\n", socket_ptr);
}

void Protocol::handleCommand130(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 130" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() < 2) {
        sendTCPMessage("$130 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::vector<int> player_ids;
    for (size_t i = 1; i < args.size(); ++i) {
        try {
            player_ids.push_back(std::stoi(args[i]));
        } catch (const std::invalid_argument &e) {
            sendTCPMessage("$130 KO: Invalid player ID format\n", socket_ptr);
            return;
        }
    }

    std::ostringstream response;
    response << "$130";
    for (const auto& player_id : player_ids) {
        std::string contentStatistics = database_.getStatisticsFromId("server/Database/table/statistic.json", player_id);
        response << " " << player_id << " " << contentStatistics;
    }
    response << "\n";

    sendTCPMessage(response.str(), socket_ptr);
}

void Protocol::handleCommand131(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 131" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 1) {
        sendTCPMessage("$131 KO: Invalid command format\n", socket_ptr);
        return;
    }

    std::string playerUsernamesConnected;

    for (const auto& player : players_) {
        if (player.second->getTCPSocket()) {
            if (!playerUsernamesConnected.empty()) {
                playerUsernamesConnected.append(", ");
            }
            std::string username = database_.getUsernameFromId(player.second->getId());
            std::string playerId = std::to_string(player.second->getId());
            playerUsernamesConnected.append(username + " - " + playerId);
        }
    }

    sendTCPMessage("$131 " + playerUsernamesConnected + "\n", socket_ptr);
}

void Protocol::handleCommand132(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 132" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 2) {
        sendTCPMessage("$132 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int room_id;
    try {
        room_id = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        sendTCPMessage("$132 KO: Invalid room ID format\n", socket_ptr);
        return;
    }

    std::ostringstream reponse;
    reponse << "$132";

    for (const auto& room : room_manager_.getRooms()) {
        if (room->getId() == room_id) {
            for (const auto& player : room->getPlayers()) {
                int player_id = player->getId();
                // std::string playerId = std::to_string(player_id);
                std::string contentStatistics = database_.getStatisticsFromId("server/Database/table/statistic.json", player_id);
                // std::cout << "Player ID: " << playerId << std::endl;
                reponse << " " << contentStatistics;
            }
            reponse << "\n";
            sendTCPMessage(reponse.str(), socket_ptr);
            std::cout << "Message envoyé via TCP : " << reponse.str() << std::endl;
            return;
        }
    }
    sendTCPMessage("$132 KO: Room not found\n", socket_ptr);
}

void Protocol::handleCommand133(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 133" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 2) {
        sendTCPMessage("$133 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int player_id_to_be_kick = std::stoi(args[1]);

    std::shared_ptr<Room> player_room = nullptr;
    std::shared_ptr<tcp::socket> player_socket = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id_to_be_kick) {
                player_room = room;
                player_socket = player->getTCPSocket();
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr || player_socket == nullptr) {
        return;
    }

    player_room->kickPlayer(player_id_to_be_kick);
    sendTCPMessage("$133 " + player_id_to_be_kick, player_socket);
}

void Protocol::handleCommand134(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 134" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 2) {
        sendTCPMessage("$134 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int player_id_to_be_ban = std::stoi(args[1]);

    std::shared_ptr<Room> player_room = nullptr;
    std::shared_ptr<tcp::socket> player_socket = nullptr;
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id_to_be_ban) {
                player_room = room;
                player_socket = player->getTCPSocket();
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    if (player_room == nullptr || player_socket == nullptr) {
        return;
    }

    player_room->kickPlayer(player_id_to_be_ban);
    database_.setBanFromId(player_id_to_be_ban, true);
    sendTCPMessage("$134 " + player_id_to_be_ban, player_socket);
}

void Protocol::handleCommand135(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling command 135" << std::endl;

    std::vector<std::string> args = parseMessage(message);

    if (args.size() != 2) {
        sendTCPMessage("$134 KO: Invalid command format\n", socket_ptr);
        return;
    }

    int player_id;
    try {
        player_id = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        sendTCPMessage("$135 KO: Invalid player ID format\n", socket_ptr);
        return;
    } catch (const std::out_of_range &e) {
        sendTCPMessage("$135 KO: Player ID out of range\n", socket_ptr);
        return;
    }

    std::string player_statistic = database_.getStatisticsFromId("server/Database/table/statistic.json", player_id);

    sendTCPMessage("135 " + player_statistic, socket_ptr);
}

void Protocol::handleTCPCommand(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr)
{
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    // Find the command handler
    auto it = tcp_command_handlers_.find(command);
    if (it != tcp_command_handlers_.end()) {
        it->second(message, socket_ptr);
    } else {
        std::cout << "Unknown command received: " << command << std::endl;
        sendTCPMessage("Unknown command\n", socket_ptr);
    }
}

void Protocol::handleUDPCommand(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::istringstream iss(message);
    std::string command;
    iss >> command;

    // Find the command handler
    auto it = udp_command_handlers_.find(command);
    if (it != udp_command_handlers_.end()) {
        it->second(message, remote_endpoint);
    } else {
        std::cout << "Unknown command received: " << command << std::endl;
        sendUDPMessage("Unknown command\n", *remote_endpoint);
    }
}

void Protocol::dumpUDP(const std::string &message, std::shared_ptr<udp::endpoint> remote_endpoint)
{
    std::cout << "Dumping UDP message: " << std::endl;
    std::cout << "\tPort: " << remote_endpoint->port() << std::endl;
    std::cout << "\tAddress: " << remote_endpoint->address().to_string() << std::endl;
    std::cout << "Players: " << std::endl;
    for (const auto& player : players_) {
        std::cout << "\tID: " << player.second->getId() << " IP: " << player.second->getIp() << " TCP Port: " << player.second->getTCPPort() << " UDP Port: " << player.second->getUDPPort() << std::endl;
    }
}

int Protocol::autoIncrementPlayerId()
{
    // A décommenter quand le systeme de login/register est mis en place
    return next_player_id_ = database_.getLastId() + 1;
    //  return next_player_id_++;
}

void Protocol::sendTCPMessage(const std::string &message, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
{
    std::string serialized_message = prepareMessage(message);

    boost::asio::async_write(*socket, boost::asio::buffer(serialized_message),
        [socket, serialized_message](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec) {
                std::cout << "Message envoyé via TCP : " << serialized_message << std::endl;
            } else {
                std::cerr << "Erreur d'envoi TCP : " << ec.message() << std::endl;
            }
        });
}

// template <typename T>
// void Protocol::sendTCPObject(const T &obj, std::shared_ptr<boost::asio::ip::tcp::socket> socket)
// {
//     std::ostringstream oss;
//     boost::archive::text_oarchive oa(oss);
//     oa << obj;
//     std::string serialized_data = oss.str();
//     sendTCPMessage(serialized_data, socket);
// }

void Protocol::tcpConnection()
{
    tcp_acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
                               {
        if (!ec) {
            std::string player_key = socket.remote_endpoint().address().to_string() + ":" + std::to_string(socket.remote_endpoint().port());

            if (players_.find(player_key) == players_.end()) {
                auto player = std::make_shared<Player>(autoIncrementPlayerId(), socket.remote_endpoint().address().to_string());
                player->setTCPPort(socket.remote_endpoint().port());
                player->setTCPSocket(std::make_shared<tcp::socket>(std::move(socket)));
                players_[player_key] = player;
                players_by_id_[player->getId()] = player;
                std::cout << "Nouveau joueur ajouté : " << player_key << std::endl;
            } else {
                if (players_[player_key]->getTCPPort() == 0) {
                    players_[player_key]->setTCPPort(socket.remote_endpoint().port());
                    players_[player_key]->setTCPSocket(std::make_shared<tcp::socket>(std::move(socket)));
                    std::cout << "Joueur existant mis à jour : " << player_key << " (TCP : " << socket.remote_endpoint().port() << ")" << std::endl;
                } else {
                    std::cout << "Joueur existant déjà connecté : " << player_key << std::endl;
                }
            }

            auto socket_ptr = players_[player_key]->getTCPSocket();
            // sendTCPMessage("Your ID " + std::to_string(players_[player_key]->getId()) + "\n", socket_ptr);
            receiveTCPMessage(socket_ptr);
        } else {
            std::cerr << "Erreur d'acceptation : " << ec.message() << std::endl;
        }
        tcpConnection(); });
}

void Protocol::handleClientDisconnect(std::shared_ptr<tcp::socket> socket_ptr)
{
    std::cout << "Handling client disconnect" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());

    if (players_.find(player_key) == players_.end()) {
        std::cerr << "Error: Player not found" << std::endl;
        return;
    }

    int player_id = players_[player_key]->getId();
    std::shared_ptr<Room> player_room = nullptr;

    // Find the room of the player
    for (const auto& room : room_manager_.getRooms()) {
        for (const auto& player : room->getPlayers()) {
            if (player->getId() == player_id) {
                player_room = room;
                break;
            }
        }
        if (player_room != nullptr) {
            break;
        }
    }

    // Notify other players in the room about the disconnection
    if (player_room != nullptr) {
        std::string disconnect_message = "$DISCONNECT Player " + std::to_string(player_id) + " has disconnected\n";
        for (const auto& player : player_room->getPlayers()) {
            if (player->getId() != player_id) {
                // sendTCPMessage(disconnect_message, player->getTCPSocket());
            }
        }
        player_room->removePlayer(players_[player_key]);
    }

    players_.erase(player_key);
    players_by_id_.erase(player_id);

    for (const auto& player_pair : players_) {
        auto player_socket = player_pair.second->getTCPSocket();
        if (player_socket) {
            sendTCPMessage("113 " + std::to_string(player_id) + "\n", player_socket);
        } else {
            std::cerr << "Error: Invalid player or player socket" << std::endl;
        }
    }
}

void Protocol::receiveTCPMessage(std::shared_ptr<tcp::socket> socket_ptr) {
    auto buffer = std::make_shared<std::array<char, 1024>>();
    socket_ptr->async_read_some(boost::asio::buffer(*buffer),
        [this, buffer, socket_ptr](boost::system::error_code ec, std::size_t bytes_transferred) {
            if (!ec) {
                try {
                    std::string message(buffer->data(), bytes_transferred);
                    std::cout << "state if serialization :" << disable_serialization_ << std::endl;
                    if (!disable_serialization_) {
                        std::istringstream iss(message);
                        boost::archive::text_iarchive ia(iss);
                        ia >> message;
                    }
                    std::cout << "Message TCP reçu : " << message << std::endl;
                    handleTCPCommand(message, socket_ptr);
                } catch (const std::exception &e) {
                    std::cerr << "Erreur de désérialisation : " << e.what() << std::endl;
                }

                receiveTCPMessage(socket_ptr);
            } else {
                std::cerr << "Erreur de lecture : " << ec.message() << std::endl;
                if (ec == boost::asio::error::eof || ec == boost::asio::error::connection_reset) {
                    handleClientDisconnect(socket_ptr);
                }
            }
        });
}

void Protocol::sendUDPMessage(const std::string &message, const udp::endpoint &remote_endpoint)
{
    std::string serialized_message;

    if (!disable_serialization_) {
        std::ostringstream archive_stream;
        boost::archive::text_oarchive archive(archive_stream);
        archive << message;
        serialized_message = archive_stream.str();
    } else {
        serialized_message = message;
    }

    udp_socket_.async_send_to(boost::asio::buffer(serialized_message), remote_endpoint,
        [serialized_message](boost::system::error_code ec, std::size_t /*bytes_sent*/)
        {
            if (!ec)
            {
                std::cout << "Message envoyé via UDP : " << serialized_message << std::endl;
            }
            else
            {
                std::cerr << "Erreur d'envoi UDP : " << ec.message() << std::endl;
            }
        });
}

void Protocol::receiveUDPMessage()
{
    auto buffer = std::make_shared<std::array<char, 1024>>();
    auto remote_endpoint = std::make_shared<udp::endpoint>();
    udp_socket_.async_receive_from(boost::asio::buffer(*buffer), *remote_endpoint,
        [this, buffer, remote_endpoint](boost::system::error_code ec, std::size_t bytes_recvd)
        {
            handleUDP(buffer, remote_endpoint, ec, bytes_recvd);
        });
}

void Protocol::handleUDP(std::shared_ptr<std::array<char, 1024>> buffer, std::shared_ptr<udp::endpoint> remote_endpoint, boost::system::error_code ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd > 0)
    {
        std::string message(buffer->data(), bytes_recvd);

        message.erase(std::remove(message.begin(), message.end(), '\r'), message.end());
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());

        handleUDPCommand(message, remote_endpoint);

        std::cout << "Données UDP reçues du port " << remote_endpoint->port() << ": " << message << std::endl;
    }
    receiveUDPMessage();
}

bool Protocol::banWordCheck(int code, const std::string& message, std::shared_ptr<tcp::socket> socket_ptr)
{
    const std::vector<std::string> bannedWords = {
        "con", "connard", "salope", "pute", "merde", "encule", 
        "fuck", "putain", "fdp", "ntm", "nique", "bite", "couille"
    };

    const std::unordered_map<char, std::string> replacements = {
        {'a', "@4àâá"}, {'e', "3éèêë"}, {'i', "1!íìî"},
        {'o', "0óòô"}, {'u', "ùúû"}, {'s', "$5"}, {'l', "1"}
    };

    std::string lowerMessage = message;
    std::transform(lowerMessage.begin(), lowerMessage.end(), lowerMessage.begin(), ::tolower);

    auto normalizeWord = [&](const std::string& word) -> std::unordered_set<std::string> {
        std::unordered_set<std::string> variations = {word};
        for (size_t i = 0; i < word.size(); ++i) {
            char c = word[i];
            if (replacements.count(c)) {
                std::string chars = replacements.at(c);
                std::unordered_set<std::string> newVariations;
                for (const auto& variant : variations) {
                    for (char alt : chars) {
                        std::string newVariant = variant;
                        newVariant[i] = alt;
                        newVariations.insert(newVariant);
                    }
                }
                variations.merge(newVariations);
            }
        }
        return variations;
    };

    std::unordered_set<std::string> normalizedBannedWords;
    for (const auto& word : bannedWords) {
        auto variants = normalizeWord(word);
        normalizedBannedWords.insert(variants.begin(), variants.end());
    }

    std::string strippedMessage;
    std::remove_copy_if(lowerMessage.begin(), lowerMessage.end(), std::back_inserter(strippedMessage), 
        [](char c) { return std::isspace(c) || std::ispunct(c); });

    for (const auto& word : normalizedBannedWords) {
        if (strippedMessage.find(word) != std::string::npos) {
            sendTCPMessage("$" + std::to_string(code) + " KO: Message contains inappropriate content\n", socket_ptr);
            return true;
        }
    }

    return false;
}

void Protocol::pingTCP(std::shared_ptr<boost::asio::ip::tcp::socket> socket) {
    sendTCPMessage("$PING\n", socket);
}

bool Protocol::verificationNextPing(Player* player) {
    auto now = std::chrono::system_clock::now();
    auto lastPing = player->getLastPing();
    
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - lastPing);
    auto ping = player->getPing();

    if (ping >= 0 && ping <= 50) {
        if (duration.count() >= 5) {
            return true;
        }
    } else if (ping >= 51 && ping <= 80) {
        if (duration.count() >= 4) {
            return true;
        }
    } else if (ping >= 81 && ping <= 100) {
        if (duration.count() >= 3) {
            return true;
        }
    } else if (ping >= 101 && ping <= 200) {
        if (duration.count() >= 2) {
            return true;
        }
    } else {
        if (duration.count() >= 1) {
            return true;
        }
    }
    
    return false;
}

void Protocol::pingLoop() {
    std::thread([this]() {
        while (true) {
            for (const auto& player : players_) {
                if (verificationNextPing(player.second.get())) {
                    player.second->startPing();
                    pingTCP(player.second->getTCPSocket());
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }).detach();
}

void Protocol::handleCommandPong(const std::string &message, std::shared_ptr<tcp::socket> socket_ptr) {
    std::cout << "Handling command PONG" << std::endl;

    std::string player_key = socket_ptr->remote_endpoint().address().to_string() + ":" + std::to_string(socket_ptr->remote_endpoint().port());
    auto player = players_[player_key];

    int ping = player->stopPing();
    player->setPing(ping);
    player->setLastPing();
    std::cout << "Ping: " << ping << "ms" << std::endl;
}