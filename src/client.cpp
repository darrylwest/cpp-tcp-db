//
// dpw
//

#include <sockpp/tcp_socket.h>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <tcpdb/base.hpp>
#include <tcpdb/client.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/types.hpp>
#include <termio/termio.hpp>

#include "sockpp/tcp_connector.h"

namespace tcpdb::client {
    using namespace termio::termio;

    std::atomic_flag quit_repl = ATOMIC_FLAG_INIT;

    Response process_request(sockpp::tcp_connector& sock, const std::string& request) {
        // std::println("send: {}", request);
        if (request == "quit") {
            quit_repl.test_and_set();
            return {"ok"};
        }

        if (request == "help") {
            return {base::help_text()};
        }

        char buf[config::BUFFER_SIZE] = {0};
        const auto req = sock.write_n(request.c_str(), request.size());
        if (req.is_error()) {
            spdlog::error("error:{} {}", req.error().value(), req.error().message());
        }

        const auto resp = sock.read(buf, sizeof(buf));
        if (resp.is_error()) {
            return {req.error().message(), 500};
        }

        const std::string text = resp.value() > 0 ? std::string(buf, resp.value()) : "ok";
        return {text};
    }

    void request_loop(sockpp::tcp_connector& sock) {
        size_t count = 1;
        while (!quit_repl.test()) {
            std::string request;
            // show the prompt
            std::print("{}{} > {}", green(), count, reset());
            std::getline(std::cin, request);
            if (request.empty()) {
                continue;
            }

            const auto resp = process_request(sock, request);
            std::println("{}", resp.text);
            count++;
        }
    }

    int start(const tcpdb::config::Config& config) {
        spdlog::info("Starting tcpdb client: {}", config.to_string());

        using namespace std::chrono;

        const auto host = config.client.server_host.c_str();
        const auto port = config.server.port;
        sockpp::initialize();

        sockpp::tcp_connector connector;  // Renamed to connector to be clear

        // Attempt to connect with a 10 sec timeout.
        if (auto res = connector.connect(host, port, 10s); !res) {
            spdlog::error("Failed to connect to {}:{}: {}", host, port, res.error().message());
            return 1;
        }

        std::println("{}socket connected on {}:{}{}", cyan(), host, port, reset());

        request_loop(connector);

        return 0;
    }

}  // namespace tcpdb::client
