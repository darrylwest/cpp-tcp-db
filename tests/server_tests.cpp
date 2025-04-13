//
// 2025-04-11 20:26:27 dpw
//
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/server.hpp>
#include <thread>

auto create_test_config() {
    tcpdb::config::Config config;
    config.server.host = "127.0.0.1";
    config.server.port = 9944;
    config.client.server_host = "127.0.0.1";
    config.client.connect_timeout = 5000;

    return config;
}

TEST_CASE("Server tests", "[configure]") {
    spdlog::set_level(spdlog::level::info);
    auto config = create_test_config();

    REQUIRE(config.server.host == "127.0.0.1");
    REQUIRE(config.server.port == 9944);

    /*
     * TODO implement this with threads t1 to run the server, t2 for client
    std::thread t([&config]() {
        auto exit_code = tcpdb::server::start(config);
        REQUIRE(exit_code == 0);
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    tcpdb::server::shutdown();

    t.join();
    */
}