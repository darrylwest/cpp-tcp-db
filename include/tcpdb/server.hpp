//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <tcpdb/config.hpp>

namespace server {
    int start(config::Config& config);
    void shutdown();
}  // namespace server
