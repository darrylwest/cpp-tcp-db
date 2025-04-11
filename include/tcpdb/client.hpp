//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>

namespace client {
    // start the repl
    int start(const tcpdb::Config& config);
    std::string help_text();
}

