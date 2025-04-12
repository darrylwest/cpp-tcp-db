//
// 2025-04-11 20:23:48 dpw
//

#pragma once

#include <string>

namespace config {
    struct Server {
        std::string host;
        int port;
        bool encrypted;
        std::string data;
        std::string logs;

        // to_string
    };

    struct Client {
        size_t connect_timeout;
        std::string logs;
        
        // to_string
    };

    struct Config {
        Server server;
        Client client;
    };
}
