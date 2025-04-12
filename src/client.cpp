//
// dpw
//

#include <spdlog/spdlog.h>

#include <tcpdb/client.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/version.hpp>

namespace tcpdb::client {
    int start(const tcpdb::config::Config& config) {
        spdlog::info("Starting tcpdb client: {}", config.to_string());

        return 0;
    }

    std::string help_text() {
        return "my help text";
    }
}
