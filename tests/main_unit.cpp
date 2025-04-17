//
// 2025-04-11 20:26:27 dpw
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function

#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <print>
#include <tcpdb/version.hpp>
#include <termio/termio.hpp>
#include <vendor/perftimer.hpp>

perftimer::PerfTimer timer("Catch2 Unit Tests");

struct MainTestSetup {
    MainTestSetup() {
        spdlog::set_level(spdlog::level::critical);

        // do any config stuff
        timer.start();
        // timer.log << "started";
    }

    ~MainTestSetup() {
        timer.stop();

        using namespace termio::termio;

        auto vers = tcpdb::Version().to_string();
        std::println("{}{}Tests complete, Version: {}{}...{}", bold(), cyan(), yellow(), vers, reset());
        timer.show_duration();
        // std::println("{}", timer.log.str());
    }
};

// put helpers here...
MainTestSetup setup;

// Tests here
TEST_CASE("Version tests", "[version]") {
    const auto vers = tcpdb::Version();

    REQUIRE(vers.major == 0);
    REQUIRE(vers.minor == 3);
    REQUIRE(vers.patch >= 3);
    REQUIRE(vers.patch <= 10);
    REQUIRE(vers.build >= 115);
    REQUIRE(vers.build <= 999);

    REQUIRE(vers.to_string().starts_with("0.3."));
}
