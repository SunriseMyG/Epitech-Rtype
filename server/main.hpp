#ifndef MAIN_HPP
#define MAIN_HPP

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::udp;


class Main {
    public:
        Main() = default;
        ~Main() = default;
        // void session(tcp::socket sock);
        void server(boost::asio::io_context &io_context, short port);
    private:
        void handle_receive(const boost::system::error_code &error, size_t bytes_transferred,
            udp::socket &socket, std::array<char, 1024> &recv_buffer, udp::endpoint &remote_endpoint);
        void start_receive(udp::socket & socket, std::array<char, 1024> & recv_buffer, udp::endpoint & remote_endpoint);

};

#endif // MAIN_HPP