//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <tcpdb/config.hpp>

namespace tcpdb::server {

    int start(const config::Config& config);
    void shutdown();
}  // namespace tcpdb::server
