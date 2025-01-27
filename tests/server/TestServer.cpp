#include <boost/asio.hpp>
#include "./../../server/Protocol/Protocol.hpp"
#include "./../../server/GameServer/GameServer.hpp"
#include <iostream>
#include <csignal>
#include <atomic>

boost::asio::io_context io_context;

int main() {
    Protocol protocol(io_context, 12345, true);
    protocol.tcpConnection();
    protocol.receiveUDPMessage();
    io_context.run();
    return 0;
}