//
// 2025-04-24 18:22:20 dpw
//

#include <spdlog/spdlog.h>

#include <fstream>
#include <nlohmann/json.hpp>
#include <tcpdb/config.hpp>

namespace tcpdb::config {
    using json = nlohmann::json;

    Config load_config() {
        Config config;
        std::string config_file = "config/config.json";

        std::ifstream ifs(config_file);

        if (!ifs.is_open()) {
            spdlog::warn("failed to open config file: {}", config_file);
            return config;
        }

        json j;
        ifs >> j;

        ifs.close();

        if (j.contains("server")) {
            const auto server = j["server"];
            config.server.host = server["host"];
            // config.server.port = server["port"].get<in_port_t>();
            int port = server["port"].get<int>();
            config.server.port = static_cast<in_port_t>(port);
            config.server.data_file = server["data_file"];
            config.server.logs = server["logs"];
            config.server.timeout_seconds = server["timeout_seconds"].get<int>();
            // config.server.encrypted = server["encrypted"];
        }

        if (j.contains("client")) {
            const auto client = j["client"];
            config.client.server_host = client["server_host"];
            config.client.logs = client["logs"];
        }

        return config;
    }
}  // namespace tcpdb::config