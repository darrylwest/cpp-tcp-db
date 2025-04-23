//
// dpw
//

#include <sockpp/tcp_socket.h>
#include <spdlog/spdlog.h>
#include <termio/termio.hpp>
#include <iostream>
#include <string>
#include <tcpdb/base.hpp>
#include <tcpdb/client.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/types.hpp>

#include "sockpp/tcp_connector.h"

namespace tcpdb::client {
    using namespace termio::termio;

    std::atomic_flag quit_repl = ATOMIC_FLAG_INIT;
    sockpp::tcp_socket sock;

    void process_request(const std::string& request) {
        std::println("send: {}", request);
        if (request == "quit") {
            quit_repl.test_and_set();
            return;
        }

        if (request == "help") {
            std::println("{}", tcpdb::base::help_text());
            return;
        }

        // send the request ; read the response
        std::string response = "my reqest: " + request;
        std::println("recv: {}", response);
    }

    void request_loop() {
        size_t count = 1;
        while (!quit_repl.test()) {
            std::string request;
            // show the prompt
            std::print("{}{} > {}", green(), count, reset());
            std::getline(std::cin, request);
            if (request.empty()) {
                continue;
            }

            process_request(request);
            count++;
        }
    }

    int start(const tcpdb::config::Config& config) {
        spdlog::info("Starting tcpdb client: {}", config.to_string());

        using namespace std::chrono;

        // connect to the server
        const auto host = "localhost"; // config.server.host.c_str();
        const auto port = config.server.port;
        sockpp::initialize();
        const auto addr = sockpp::inet_address(host, port);

        if (!sock.bind(addr)) {
            spdlog::error("Could not connect to server: {}", config.server.host);
            return 1;
        }

        std::println("{}socket connected{}", cyan(), reset());

        request_loop();

        return 0;
    }

}  // namespace tcpdb::client
