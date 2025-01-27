
#ifndef TEST_CLIENT_HPP
    #define TEST_CLIENT_HPP

    #include <boost/asio.hpp>
    #include <string>
    #include <memory>
    #include <thread>
    #include <queue>
    #include <mutex>
    #include <vector>
    #include <iostream>
    #include <boost/asio/ip/tcp.hpp>
    #include <boost/asio/ip/udp.hpp>


    using boost::asio::ip::tcp;
    using boost::asio::ip::udp;

    class TestClient {
        public:
            TestClient();
            ~TestClient() = default;
            void connect();
            void sendTCP(const std::string& message);
            void receiveTCP(const std::string &message);
        private:
            std::shared_ptr<boost::asio::ip::tcp::socket> tcp_socket;
            boost::asio::io_context io_context;
            std::string host = "localhost";
            std::string port = "12345";
            std::thread receive_thread;
    };

#endif // TEST_CLIENT_HPP