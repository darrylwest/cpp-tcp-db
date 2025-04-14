//
// 2025-04-14 14:25:31 dpw
//

#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <string>
#include <tcpdb/termio.hpp>
#include <vector>

TEST_CASE("Termio tests", "[wrap]") {
    spdlog::set_level(spdlog::level::info);
    using namespace tcpdb::termio;

    const auto text = wrap("This is blue text.", Color::blue, Attr::bold);

    spdlog::info("{}", text);

    REQUIRE(text.contains("This is blue text."));
    REQUIRE(text.starts_with(to_string(Attr::bold)));
    REQUIRE(text.contains(to_string(Color::blue)));
    REQUIRE(text.ends_with(to_string(Color::reset)));

    spdlog::set_level(spdlog::level::critical);
}

TEST_CASE("Termio tests", "[wrap][default-attr]") {
    spdlog::set_level(spdlog::level::info);
    using namespace tcpdb::termio;

    const auto text = wrap("This is yellow text.", Color::yellow);

    spdlog::info("{}", text);

    REQUIRE(text.contains("This is yellow text."));
    REQUIRE(text.starts_with(to_string(Attr::normal)));
    REQUIRE(text.contains(to_string(Color::yellow)));
    REQUIRE(text.ends_with(to_string(Color::reset)));

    spdlog::set_level(spdlog::level::critical);
}

TEST_CASE("Termio tests", "[colors]") {
    using namespace tcpdb::termio;
    std::ostringstream oss;

    oss << bold() << "some text" << reset_nl();
    auto text = oss.str();

    REQUIRE(text.starts_with(to_string(Attr::bold)));
    REQUIRE(text.contains("some text"));
    REQUIRE(text.ends_with(to_string(Color::reset) + "\n"));
}