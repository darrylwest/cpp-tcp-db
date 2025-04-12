//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>

namespace tcpdb::config {
    struct Server {
        std::string host = "0.0.0.0";
        int port = 14500;
        bool encrypted = false;
        std::string data = "data/server.db";
        std::string logs = "logs/server.log";

        [[nodiscard]] auto to_string() const -> std::string {
            return fmt::format("host: {} port: {}", host, port);
        }
    };

    struct Client {
        size_t connect_timeout = 5000;
        std::string logs = "logs/client.log";

        [[nodiscard]] auto to_string() const -> std::string {
            return fmt::format("timeout: {} logs: {}", connect_timeout, logs);
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
}  // namespace config
