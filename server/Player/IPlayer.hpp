#ifndef IPLAYER_HPP
#define IPLAYER_HPP

#include <string>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class IPlayer {
    public:
        virtual ~IPlayer() = default;

        virtual int getId() = 0;
        virtual std::string getIp() = 0;
        virtual unsigned short getTCPPort() = 0;
        virtual unsigned short getUDPPort() = 0;
        virtual void setIp(const std::string &ip) = 0;
        virtual void setTCPPort(unsigned short port) = 0;
        virtual void setUDPPort(unsigned short port) = 0;
        virtual void setUDPRemote(std::shared_ptr<udp::endpoint> udp_remote) = 0;
        virtual std::shared_ptr<udp::endpoint> getUDPRemote() = 0;
};

#endif // IPLAYER_HPP