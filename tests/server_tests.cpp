//
// 2025-04-11 20:26:27 dpw
//
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <tcpdb/base.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>
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
    auto config = create_test_config();

    REQUIRE(config.server.host == "127.0.0.1");
    REQUIRE(config.server.port == 9944);
}

TEST_CASE("Server tests", "[api-request][version]") {
    auto resp = tcpdb::server::handle_request("version");
    auto version = tcpdb::Version().to_string();

    REQUIRE(resp.text == version);
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}

TEST_CASE("Server tests", "[api-request][help]") {
    auto resp = tcpdb::server::handle_request("help");
    auto help_text = tcpdb::base::help_text();

    REQUIRE(resp.text == help_text);
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}
