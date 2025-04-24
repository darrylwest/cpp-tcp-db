//
// Created by Darryl West on 4/11/25.
//
#include <catch2/catch_all.hpp>

#include "nlohmann/json.hpp"
#include "tcpdb/config.hpp"

TEST_CASE("Config tests", "[server]") { REQUIRE(true); }

TEST_CASE("Config tests", "[client]") { REQUIRE(true); }

TEST_CASE("Config tests", "[config][read]") {
    using namespace nlohmann;
    tcpdb::config::Config config;

    REQUIRE(true);
}
