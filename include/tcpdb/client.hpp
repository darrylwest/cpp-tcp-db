//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>
#include <tcpdb/config.hpp>

namespace tcpdb::client {
    int start(const config::Config& config);
    std::string help_text();
}  // namespace tcpdb::client
