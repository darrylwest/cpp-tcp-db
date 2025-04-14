//
// 2025-04-11 20:26:27 dpw
//
#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <string>
#include <tcpdb/base.hpp>
#include <vector>

TEST_CASE("Base tests", "[remove_newline]") {
    std::string text = "this is a test\r\n";
    tcpdb::base::remove_newlines(text);

    REQUIRE(text == "this is a test");

    text = "this \nis a new test\r\n";
    tcpdb::base::remove_newlines(text);

    REQUIRE(text == "this is a new test");
}

TEST_CASE("Base tests", "[join]") {
    std::vector<std::string> tests = {"k1", "k2", "k3", "k4"};
    auto line = tcpdb::base::join(tests);

    REQUIRE(line == "k1,k2,k3,k4");
}

TEST_CASE("Base tests", "[help text]") {
    auto text = tcpdb::base::help_text();

    REQUIRE(text.contains("tcp-db, version:"));
    REQUIRE(text.size() > 600);
    REQUIRE(text.size() < 1000);
}

TEST_CASE("Base tests", "[screen]") {

    // auto line = colors::green + "This is a test\n" + colors::reset;

    REQUIRE(true);
}
