//
// 2025-04-11 20:26:27 dpw
//
#include <catch2/catch_all.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/client.hpp>

TEST_CASE("Client tests", "[configure]") { REQUIRE(true); }

TEST_CASE("Client tests", "[start]") { REQUIRE(true); }

TEST_CASE("Client tests", "[help_text]") {
    auto help = tcpdb::client::help_text();
    REQUIRE(help.size() > 5);
}
