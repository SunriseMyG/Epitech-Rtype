
#ifndef PARSING_HPP
#define PARISNG_HPP

#include <iostream>

class Parsing {
    public:
        Parsing() = default;
        ~Parsing() = default;
        void parse(int argc, char **argv);
        void help();
    private:
        std::string data;
};

#endif // PARSING_HPP
