//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>
#include <tcpdb/config.hpp>

namespace tcpdb::client {
    // start the client connection socket
    int start(const config::Config& config);

    // process the user's request
    void process_request(const std::string& request);

    void request_loop();
}  // namespace tcpdb::client
