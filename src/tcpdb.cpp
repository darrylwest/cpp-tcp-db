//
// dpw
//

#include <spdlog/spdlog.h>
#include <unistd.h>

#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>

// configure the server logs


int main() {
    const auto pid = getpid();
    const auto vers = tcpdb::Version();
    tcpdb::config::Config config;

    // TODO configure logging and data

    spdlog::info("tcp-db server, version: {}, pid: {}", vers.to_string(), pid);

    auto exit_code = tcpdb::server::start(config);

    return exit_code;
}
