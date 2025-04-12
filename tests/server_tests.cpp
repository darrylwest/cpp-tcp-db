//
// 2025-04-11 20:26:27 dpw
//
#include <catch2/catch_all.hpp>

#include <spdlog/spdlog.h>
#include <tcpdb/config.hpp>
#include <tcpdb/server.hpp>

auto create_test_config() {
    tcpdb::config::Config config;
    config.server.host = "localhost";
    config.server.port = 9944;
    config.client.connect_timeout = 5000;

    return config;
}

TEST_CASE("Server tests", "[configure]") {
    tcpdb::config::Config config;

    auto exit_code = tcpdb::server::start(config);

    REQUIRE(exit_code == 0);
}