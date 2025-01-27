
#include "main.hpp"
#include "Network/Network.hpp"

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <ip_address> <port>" << std::endl;
        return 84;
    }

    // check port validity
    try {
        std::stoi(argv[2]);
    } catch (const std::exception& e) {
        std::cerr << "Invalid port: " << argv[2] << std::endl;
        return 84;
    }

    Window window;

    try {
        window.createWindow(argv[1], argv[2]);

    } catch (const std::exception& e) {
        std::cerr << "Failed to connect to server: " << e.what() << std::endl;
        return 84;
    }



    // Window window;

    // window.createWindow();
    
    return 0;
}
