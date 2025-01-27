#include "GameServer.hpp"
#include "../Room/Room.hpp"
#include "../RoomManager/RoomManager.hpp"

GameServer::GameServer(boost::asio::io_context &io_context, short port)
    : io_context_(io_context), timer_(io_context), ping_timer_(io_context), protocol_(io_context, port)
{
    // Création de deux rooms test
    auto room1 = std::make_shared<Room>(10, "surnai", "", Room::State::WAITING, 4, true);
    auto room2 = std::make_shared<Room>(11, "braa", "password123", Room::State::WAITING, 2, true);
    auto room3 = std::make_shared<Room>(12, "bull", "password123", Room::State::WAITING, 4, true);

    protocol_.getRoomManager().addRoom(room1);
    protocol_.getRoomManager().addRoom(room2);
    protocol_.getRoomManager().addRoom(room3);

    protocol_.getRoomManager().dumpRooms();

    protocol_.tcpConnection();
    protocol_.receiveUDPMessage();
//     start_timer();
//     start_ping_timer();
}


// void GameServer::handle_tcp_message(std::shared_ptr<tcp::socket> socket, const std::string &message)
// {
//     if (message == "start")
//     {
//         std::string response = "game starting";
//         boost::asio::async_write(*socket, boost::asio::buffer(response),
//                                  [this, socket](boost::system::error_code ec, std::size_t /*length*/)
//                                  {
//                                      if (!ec)
//                                      {
//                                          std::cout << "Message envoyé au client : game starting" << std::endl;
//                                      }
//                                  });
//     }
// }

// void GameServer::handle_receive_udp(const std::string &message, const udp::endpoint &remote_endpoint)
// {
//     if (message == "joinroom")
//     {
//         auto player_id = std::to_string(remote_endpoint.port());
//         auto player_it = std::find_if(player_objects_.begin(), player_objects_.end(),
//                                       [&player_id](const std::shared_ptr<Player> &player)
//                                       {
//                                           return std::to_string(player->getId()) == player_id;
//                                       });
//         if (player_it != player_objects_.end())
//         {
//             auto room = room_manager_.getRoom(1);
//             if (!room)
//             {
//                 room = std::make_shared<Room>(room_manager_.addRoomIncrement(), "Room " + std::to_string(room_manager_.getRoomIncrement()), "", Room::State::WAITING, 4);
//                 room_manager_.addRoom(room);
//             }
//             room_manager_.playerJoinRoom(*player_it, room->getId());
//             room_manager_.dumpRooms();

//             // Envoyer une réponse au client
//             std::string response = "joined room " + std::to_string(room->getId());
//             udp_socket_->async_send_to(boost::asio::buffer(response), remote_endpoint,
//                                        [](boost::system::error_code ec, std::size_t /*bytes_sent*/)
//                                        {
//                                            if (!ec)
//                                            {
//                                                std::cout << "Réponse envoyée au client : joined room" << std::endl;
//                                            }
//                                        });
//         }
//     }
// }

// void GameServer::send_update_to_players(const std::string &update, const udp::endpoint &exclude_endpoint)
// {
//     for (const auto &[_, endpoint] : players_)
//     {
//         if (endpoint != exclude_endpoint)
//         {
//             udp_socket_->async_send_to(boost::asio::buffer(update), endpoint,
//                                        [](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
//         }
//     }
// }

// void GameServer::start_timer()
// {
//     timer_.expires_after(std::chrono::seconds(5));
//     timer_.async_wait([this](const boost::system::error_code &ec)
//                       {
//         if (!ec) {
//             std::string message = "Message périodique du serveur\n";
//             for (const auto& [_, endpoint] : players_) {
//                 udp_socket_->async_send_to(boost::asio::buffer(message), endpoint,
//                     [](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
//             }
//             start_timer();
//         } });
// }

// void GameServer::start_ping_timer()
// {
//     ping_timer_.expires_after(std::chrono::seconds(5));
//     ping_timer_.async_wait([this](const boost::system::error_code &ec)
//                            {
//         if (!ec) {
//             auto now = std::chrono::steady_clock::now();
//             for (auto it = players_.begin(); it != players_.end();) {
//                 std::string player_id = it->first;
//                 auto last_ping_time = last_ping_[player_id];
//                 if (std::chrono::duration_cast<std::chrono::seconds>(now - last_ping_time).count() > 10) {
//                     std::cout << "Joueur déconnecté : " << player_id << std::endl;
//                     it = players_.erase(it);
//                     last_ping_.erase(player_id);
//                 } else {
//                     udp_socket_->async_send_to(boost::asio::buffer("ping"), it->second,
//                         [](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
//                     ++it;
//                 }
//             }
//             start_ping_timer();
//         } });
// }

// void GameServer::handle_ping_response(const udp::endpoint &endpoint)
// {
//     std::string player_id = std::to_string(endpoint.port());
//     last_ping_[player_id] = std::chrono::steady_clock::now();
//     std::cout << "Ping reçu de " << player_id << std::endl;
// }

// void GameServer::getUserPort()
// {
//     std::cout << "Table des ports des utilisateurs connectés :" << std::endl;
//     for (const auto &[player_id, endpoint] : players_)
//     {
//         std::cout << "Player " << player_id << " is connected on port " << endpoint.port() << std::endl;
//     }
//     std::cout << std::endl;
// }