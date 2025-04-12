//
// dpw
//

#include <spdlog/spdlog.h>

#include <tcpdb/client.hpp>
#include <tcpdb/version.hpp>

int main() {
    const auto vers = tcpdb::Version();
    spdlog::info("Hello and welcome to the tcp-db client application, version: {}", vers.to_string());

    return 0;
}
