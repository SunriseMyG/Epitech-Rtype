
#ifndef GAMESERVER_HPP
    #define GAMESERVER_HPP

    #include <iostream>
    #include <boost/asio.hpp>
    #include <boost/asio/steady_timer.hpp>
    #include <unordered_map>
    #include <memory>
    #include <vector>
    #include <map>
    #include "../Protocol/Protocol.hpp"
    #include "../RoomManager/RoomManager.hpp"
    #include "../Player/Player.hpp"

    using boost::asio::ip::tcp;
    using boost::asio::ip::udp;


    class GameServer{

        public:
            GameServer(boost::asio::io_context &io_context, short port);
            ~GameServer() = default;

        private:
            void start_receive_udp();
            void send_update_to_players(const std::string &update, const udp::endpoint &exclude_endpoint);
            void start_timer();
            void start_ping_timer();
            void handle_ping_response(const udp::endpoint &endpoint);
            void handle_receive_udp(const std::string &message, const udp::endpoint &remote_endpoint);
            void handle_tcp_message(std::shared_ptr<tcp::socket> socket, const std::string &message);
            int autoIncrementPlayerId() { return playerIncrement++; }

            // getter
            void getUserPort();

            boost::asio::io_context &io_context_;
            boost::asio::steady_timer timer_;
            boost::asio::steady_timer ping_timer_;
            std::unordered_map<std::string, udp::endpoint> players_;
            std::unordered_map<std::string, std::chrono::steady_clock::time_point> last_ping_;
            std::vector<std::shared_ptr<Player>> player_objects_;
            RoomManager room_manager_;

            int playerIncrement = 1;

            Protocol protocol_;
    };

#endif // GAMESERVER_HPP
