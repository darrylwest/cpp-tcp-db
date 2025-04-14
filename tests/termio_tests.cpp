//
// 2025-04-14 14:25:31 dpw
//

#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <string>
#include <tcpdb/termio.hpp>
#include <vector>

TEST_CASE("Termio tests", "[wrap]") {
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
    using namespace tcpdb::termio;

    const auto text = wrap("This is yellow text.", Color::yellow);

    spdlog::info("{}", text);

    REQUIRE(text.contains("This is yellow text."));
    REQUIRE(text.starts_with(to_string(Attr::normal)));
    REQUIRE(text.contains(to_string(Color::yellow)));
    REQUIRE(text.ends_with(to_string(Color::reset)));

    spdlog::set_level(spdlog::level::critical);
}

TEST_CASE("Termio tests", "[attrs][bold]") {
    using namespace tcpdb::termio;
    std::ostringstream oss;

    oss << bold() << "some text" << reset_nl();
    auto text = oss.str();

    REQUIRE(text.starts_with(to_string(Attr::bold)));
    REQUIRE(text.contains("some text"));
    REQUIRE(text.ends_with(to_string(Color::reset) + "\n"));
}

TEST_CASE("Termio tests", "[colors][bright]") {
    using namespace tcpdb::termio;

    const auto bright_blue = to_string(Color::blue, true);
    spdlog::info("{}bright blue{}", bright_blue, reset());

    const auto blue = to_string(Color::blue);

    spdlog::info("{}normal blue{}", blue, reset());
    REQUIRE(true);

    spdlog::set_level(spdlog::level::critical);
}

TEST_CASE("Termio tests", "[colors][all-normal]") {
    using namespace tcpdb::termio;

    REQUIRE(black() == to_string(Color::black));
    REQUIRE(red() == to_string(Color::red));
    REQUIRE(green() == to_string(Color::green));
    REQUIRE(yellow() == to_string(Color::yellow));
    REQUIRE(blue() == to_string(Color::blue));
    REQUIRE(magenta() == to_string(Color::magenta));
    REQUIRE(cyan() == to_string(Color::cyan));
    REQUIRE(white() == to_string(Color::white));
}
