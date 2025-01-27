#include <iostream>
#include <boost/asio.hpp>
#include "./GameServer/GameServer.hpp"
#include "./Parsing/Parsing.hpp"

int main(int argc, char **argv) {
    Parsing parsing;
    parsing.parse(argc, argv);

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    auto port = atoi(argv[1]);

    while (true) {
        try {
            boost::asio::io_context io_context;
            GameServer game_server(io_context, port);
            std::cout << "Serveur démarré sur le port TCP et UDP " << port << std::endl;
            io_context.run();
        } catch (const boost::system::system_error& e) {
            if (e.code() == boost::asio::error::not_connected) {
                std::cerr << "Connection error: " << e.what() << std::endl;
                std::cout << "Attempting to restart server..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                continue;
            }
            std::cerr << "Fatal error: " << e.what() << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cerr << "Fatal error: " << e.what() << std::endl;
            break;
        }
    }

    return 0;
}