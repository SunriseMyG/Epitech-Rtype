#include "Parsing.hpp"
#include "./../Errors/ErrorsParsing.hpp"

void Parsing::help()
{
    std::cout << "USAGE ./r-type_server port" << std::endl;
    std::cout << "\tport is the port number on which the server socket listens." << std::endl;
}

void Parsing::parse(int argc, char **argv)
{
    try {
        if (argc != 2) {
            throw ErrorsParsing("Invalid number of arguments");
        }

        if (std::string(argv[1]).find_first_not_of("0123456789") != std::string::npos) {
            throw ErrorsParsing("Invalid port number");
        }
        
        if (std::stoi(argv[1]) < 0 || std::stoi(argv[1]) > 65535) {
            throw ErrorsParsing("Invalid port number");
        }

    } catch (ErrorsParsing &e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    }

    if (std::string(argv[1]).compare("--help") == 0) {
        help();
        exit(0);
    }
}
