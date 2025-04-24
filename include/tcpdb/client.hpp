//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <sockpp/tcp_connector.h>

#include <string>
#include <tcpdb/config.hpp>

namespace tcpdb::client {
    struct Response {
        std::string text;
        int error_code = 0;
        bool quit = false;
    };

    // start the client connection socket
    int start(const config::Config& config);

    // process the user's request
    Response process_request(sockpp::tcp_connector& sock, const std::string& request);

    void request_loop(sockpp::tcp_connector& sock);
}  // namespace tcpdb::client
