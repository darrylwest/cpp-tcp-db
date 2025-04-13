//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <sockpp/tcp_socket.h>

#include <tcpdb/config.hpp>

namespace tcpdb::server {

    struct Response {
        std::string text;
        int error_code = 0;
        bool shutdown = false;
        bool quit = false;

        // Default constructor
        Response() = default;

        // Constructor with text only
        Response(std::string txt) : text(std::move(txt)) {}

        // Constructor with text and error_code
        Response(std::string txt, int code) : text(std::move(txt)), error_code(code) {}

        Response(std::string txt, int code, bool shutdown_flag, bool quit_flag)
            : text(std::move(txt)), error_code(code), shutdown(shutdown_flag), quit(quit_flag) {}
    };

    int start(const config::Config& config);
    void shutdown();
    void handle_client(sockpp::tcp_socket sock);
    Response handle_request(const std::string& request);
}  // namespace tcpdb::server
