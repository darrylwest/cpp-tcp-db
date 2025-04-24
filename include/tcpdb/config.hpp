//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <spdlog/fmt/fmt.h>

#include <string>
#include <tcpdb/types.hpp>

namespace tcpdb::config {
    constexpr size_t BUFFER_SIZE = 10000;

    struct Server {
        std::string host = "0.0.0.0";
        in_port_t port = 14500;
        bool encrypted = false;
        std::string data_file = "data/store.db";
        std::string logs = "logs/server.log";
        int timeout_seconds = 5;

        [[nodiscard]] auto to_string() const -> std::string {
            return fmt::format("host: {} port: {}", host, port);
        }
    };

    struct Client {
        std::string server_host = "10.0.1.237";
        size_t connect_timeout = 5000;
        std::string logs = "logs/client.log";

        [[nodiscard]] auto to_string() const -> std::string {
            return fmt::format("host: {} timeout: {} logs: {}", server_host, connect_timeout, logs);
        }
    };

    struct Config {
        Server server;
        Client client;

        [[nodiscard]] auto to_string() const -> std::string {
            return fmt::format("server: {}, client: {}", server.to_string(), client.to_string());
        }
    };

    Config load_config();
}  // namespace tcpdb::config
