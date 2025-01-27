

#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include "IErrors.hpp"

class Errors
{
public:
    explicit Errors(const std::string &message) : _message(message) {}
    ~Errors() = default;
    virtual const char *what() const noexcept { return _message.c_str(); }

private:
    std::string _message;
};
#endif /* !ERRORS_HPP_ */