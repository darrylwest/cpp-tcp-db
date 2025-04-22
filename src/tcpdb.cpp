//
// dpw
//

#include <spdlog/spdlog.h>
#include <unistd.h>

#include <print>
#include <tcpdb/base.hpp>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>
#include <termio/termio.hpp>

#include "spdlog/sinks/rotating_file_sink.h"

constexpr int LOG_SIZE = 1'000'000;

// configure the server logs
void configure_logs(const std::string& logfile, const bool rolling) {
    if (rolling) {
        auto logger = spdlog::rotating_logger_mt("tcpdb", logfile, LOG_SIZE, 3);
        spdlog::set_default_logger(logger);
    }

    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
}

const char* BANNER = R"(
___________________________________
   __                       ____
  / /__________        ____/ / /_
 / __/ ___/ __ \______/ __  / __ \
/ /_/ /__/ /_/ /_____/ /_/ / /_/ /
\__/\___/ .___/      \__,_/_.___/
       /_/
___________________________________
)";

int main() {
    const auto pid = getpid();
    const auto vers = tcpdb::Version();

    using namespace termio::termio;
    std::println("{}{}{}", green(), BANNER, reset());
    std::println("{}Version: {}, {}", cyan(), vers.to_string(), reset());
    std::println("{}pid: {}{}", yellow(), pid, reset());

    tcpdb::config::Config config;

    std::println("{}logs: {}{}", green(), config.server.logs, reset());
    configure_logs(config.server.logs, true);

    spdlog::info("tcp-db server, version: {}, pid: {}", vers.to_string(), pid);

    auto exit_code = tcpdb::server::start(config);

    return exit_code;
}
