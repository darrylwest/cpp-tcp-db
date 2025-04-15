//
// 2025-04-11 20:26:27 dpw
//
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <domainkeys/keys.hpp>
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

TEST_CASE("Server tests", "[api-request][ping]") {
    auto resp = tcpdb::server::handle_request("ping");
    auto version = tcpdb::Version().to_string();

    REQUIRE(resp.text == "pong");
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

TEST_CASE("Server test", "[api-request][get,set,remove]") {

    auto resp = tcpdb::server::handle_request("get flarb-not-found");
    INFO("response text: " + resp.text);
    REQUIRE(resp.text.contains("not found"));
    REQUIRE(resp.error_code >= 400);
    REQUIRE(resp.error_code < 500);

    auto key = domainkeys::keys::create_route_key().to_string();
    auto value = "this is a test value";
    auto oss = tcpdb::base::create_oss();

    oss << "set " << key << " " << value << '\n';

    resp = tcpdb::server::handle_request(oss.str());
    REQUIRE(resp.text.contains("ok"));
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    oss = tcpdb::base::create_oss();
    oss << "get " << key << '\n';
    INFO(oss.str());
    resp = tcpdb::server::handle_request(oss.str());
    REQUIRE(resp.text == value);
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    oss = tcpdb::base::create_oss();
    oss << "remove " << key << '\n';
    INFO(oss.str());
    resp = tcpdb::server::handle_request(oss.str());
    REQUIRE(resp.text == "ok");
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    oss = tcpdb::base::create_oss();
    oss << "get " << key << '\n';
    INFO(oss.str());
    resp = tcpdb::server::handle_request(oss.str());
    INFO("response text: " + resp.text);
    REQUIRE(resp.text.contains("not found"));
    REQUIRE(resp.error_code >= 400);
    REQUIRE(resp.error_code < 500);
}
