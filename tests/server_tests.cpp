//
// 2025-04-11 20:26:27 dpw
//
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <domainkeys/keys.hpp>
#include <filesystem>
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
    config.server.data_file = "data/test.db";

    tcpdb::server::set_store_path(config.server.data_file);

    return config;
}

TEST_CASE("Server tests", "[configure]") {
    auto config = create_test_config();

    REQUIRE(config.server.host == "127.0.0.1");
    REQUIRE(config.server.port == 9944);
    REQUIRE(config.server.data_file == "data/test.db");
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

TEST_CASE("Server test", "[api-request][dbsize,set txkey]") {
    auto resp = tcpdb::server::handle_request("dbsize");

    INFO("response text: " + resp.text);

    REQUIRE(resp.text == "0");
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    for (int i = 0; i < 10; i++) {
        auto value = "this is a test value with auto-generated key";
        auto oss = tcpdb::base::create_oss();
        oss << "set " << "txkey " << value << '\n';
        INFO(oss.str());
        resp = tcpdb::server::handle_request(oss.str());
        REQUIRE(resp.text.starts_with("ok"));
        REQUIRE(resp.error_code == 0);
        REQUIRE(resp.quit == false);
        REQUIRE(resp.shutdown == false);
    }

    resp = tcpdb::server::handle_request("dbsize,cleardb");

    INFO("response text: " + resp.text);

    REQUIRE(resp.text == "10");
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    resp = tcpdb::server::handle_request("cleardb");
    INFO("response text: " + resp.text);
    REQUIRE(resp.text == "ok");
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    resp = tcpdb::server::handle_request("dbsize");
    INFO("response text: " + resp.text);
    REQUIRE(resp.text == "0");
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}

TEST_CASE("Server test", "[api-request][keys]") {
    auto resp = tcpdb::server::handle_request("keys");

    REQUIRE(resp.text.empty());
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    auto key = domainkeys::keys::create_route_key().to_string();
    auto value = "this is a test value";
    auto oss = tcpdb::base::create_oss();
    oss << "set " << key << " " << value << '\n';
    INFO(oss.str());
    resp = tcpdb::server::handle_request(oss.str());
    REQUIRE(resp.text.starts_with("ok"));
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);

    resp = tcpdb::server::handle_request("keys");
    INFO("response text: " + resp.text);
    REQUIRE(resp.text == key);

    oss = tcpdb::base::create_oss();
    oss << "remove " << key << '\n';
    INFO(oss.str());
    resp = tcpdb::server::handle_request(oss.str());
    REQUIRE(resp.text.starts_with("ok"));
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}

TEST_CASE("Server test", "[api-request][status]") {
    auto resp = tcpdb::server::handle_request("status");

    INFO("response text: " + resp.text);

    REQUIRE(resp.text.contains("ok"));
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}

TEST_CASE("Server test", "[api-request][save]") {
    // this will set the store's test database
    create_test_config();

    try {
        std::filesystem::create_directory("data");
    } catch (...) {
    }

    auto resp = tcpdb::server::handle_request("save");

    INFO("response text: " + resp.text);

    REQUIRE(resp.text.contains("ok"));
    REQUIRE(resp.error_code == 0);
    REQUIRE(resp.quit == false);
    REQUIRE(resp.shutdown == false);
}
