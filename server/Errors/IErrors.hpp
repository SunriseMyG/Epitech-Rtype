

#ifndef IERRORS_HPP_
#define IERRORS_HPP_

#include <stdexcept>
#include <string>

class IErrors : public std::exception
{
public:
    explicit IErrors(const std::string &message) : _message(message) {}
    virtual ~IErrors() = default;
    virtual const char *what() const noexcept override { return _message.c_str(); }

private:
    std::string _message;
};

#endif /* !IERRORS_HPP_ */