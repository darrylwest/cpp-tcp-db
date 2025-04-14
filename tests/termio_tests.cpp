//
// 2025-04-14 14:25:31 dpw
//

#include <spdlog/spdlog.h>
// #include <spdlog/fmt/fmt.h>
#include <catch2/catch_all.hpp>
#include <string>
#include <tcpdb/termio.hpp>
#include <vector>

TEST_CASE("Termio tests", "[colors]") {
    using namespace tcpdb::termio;

    // std::string text = fmt::format("{}This is a {}blue blob{} of text.{}\n", Attr::bold, Color::blue, Color::reset, Color::reset);

    REQUIRE(true);
}
