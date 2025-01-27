#define BOOST_TEST_MODULE ProtocolTest
#include <boost/test/included/unit_test.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <nlohmann/json.hpp>
#include <chrono>
#include <iomanip>

using json = nlohmann::json;

struct TestClientFixture {
    boost::asio::io_context io_context;
    boost::asio::ip::tcp::socket socket;

    TestClientFixture() : socket(io_context) {
        boost::asio::ip::tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("127.0.0.1", "12345");
        
        boost::asio::connect(socket, endpoints);
        std::cout << "Connected to server" << std::endl;
    }

    ~TestClientFixture() {
        socket.close();
    }

    void print_header(const std::string& test_name) {
        std::cout << "\n==============================\n";
        std::cout << "TEST: " << test_name << "\n";
        std::cout << "==============================\n";
    }

    void print_message(const std::string& label, const std::string& message) {
        std::cout << label << ": " << message << "\n";
    }

    void print_elapsed_time(double elapsed) {
        std::cout << "Elapsed time: " << std::fixed << std::setprecision(6) << elapsed << " seconds\n";
    }

    void send_message(const std::string& message, const std::string& expected_response) {
        auto start = std::chrono::high_resolution_clock::now();

        boost::asio::write(socket, boost::asio::buffer(message));
        print_message("Message sent", message);

        std::array<char, 128> buf;
        size_t len = socket.read_some(boost::asio::buffer(buf));
        std::string response(buf.data(), len);
        print_message("Response received", response);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        print_elapsed_time(elapsed.count());

        BOOST_REQUIRE_EQUAL(response, expected_response);
    }

    void send_message_and_compare_json(const std::string& message, const json& expected_json) {
        auto start = std::chrono::high_resolution_clock::now();

        boost::asio::write(socket, boost::asio::buffer(message));
        print_message("Message sent", message);

        std::array<char, 512> buf;
        size_t len = socket.read_some(boost::asio::buffer(buf));
        std::string response(buf.data(), len);
        print_message("Response received", response);

        // Extraire la partie JSON de la réponse
        std::string json_str = response.substr(response.find(' ') + 1);
        json received_json = json::parse(json_str);

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        print_elapsed_time(elapsed.count());

        BOOST_REQUIRE_EQUAL(received_json.dump(), expected_json.dump());
    }
};

BOOST_FIXTURE_TEST_SUITE(TestClientSuite, TestClientFixture)

BOOST_AUTO_TEST_CASE(test_send_100) {
    print_header("Connect to server");
    send_message("100\n", "$100 1\n");
}

BOOST_AUTO_TEST_CASE(test_send_102) {
    print_header("Create a room");
    send_message("102 room_name room_password 4\n", "$102 OK\n");
}

BOOST_AUTO_TEST_CASE(test_send_103) {
    print_header("Join a room");
    send_message("103 1 room_password\n", "$103 OK\n");
}

BOOST_AUTO_TEST_CASE(test_send_104) {
    json expected_json = {
        {"1", {
            {"room name", "room_name"},
            {"room password", "room_password"},
            {"room state", "0"},
            {"max players", "4"},
            {"current players", "0"}
        }}
    };

    print_header("Get all rooms");
    send_message_and_compare_json("104\n", expected_json);
}

BOOST_AUTO_TEST_CASE(test_send_112) {
    print_header("Leave a room");
    send_message("103 1 room_password\n", "$103 OK\n");
    send_message("112 1\n", "$112 OK\n");
}

BOOST_AUTO_TEST_SUITE_END()