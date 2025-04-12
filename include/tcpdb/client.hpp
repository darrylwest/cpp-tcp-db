//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>
#include <tcpdb/config.hpp>

namespace client {
    int start(config::Config& config);
    std::string help_text();
}  // namespace client
