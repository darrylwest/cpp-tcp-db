//
// Created by Darryl West on 3/22/25.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function

#include <catch2/catch_all.hpp>
#include <app/tiny.hpp>
#include <app/version.hpp>
#include <vendor/ansi_colors.hpp>
#include <spdlog/spdlog.h>
#include <print>
#include <vendor/perftimer.hpp>

perftimer::PerfTimer timer("Catch2 Unit Tests");

struct MainTestSetup {

    MainTestSetup() {
        using namespace colors;
        spdlog::set_level(spdlog::level::critical);

        // do any config stuff
        timer.start();
        // timer.log << "started";
    }

    ~MainTestSetup() {
        using namespace colors;
        timer.stop();
        std::println("{}Tests complete...{}", bright::cyan, colors::reset);
        timer.show_duration();
        // std::println("{}", timer.log.str());
    }
};

// put helpers here...
MainTestSetup setup;

// Tests here
TEST_CASE("Version test", "[version]") {
    const auto vers = app::Version();
    REQUIRE(vers.major == 0);
    REQUIRE(vers.minor == 1);
    REQUIRE(vers.patch == 1);
    REQUIRE(vers.build >= 100);
    REQUIRE(vers.to_string().starts_with("0.1.1"));
}
