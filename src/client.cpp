//
// dpw
//

#include <spdlog/spdlog.h>

#include <iostream>
#include <tcpdb/base.hpp>
#include <tcpdb/client.hpp>
#include <tcpdb/config.hpp>
#include <tcpdb/types.hpp>
#include <tcpdb/version.hpp>

namespace tcpdb::client {

    std::atomic_flag quit_repl = ATOMIC_FLAG_INIT;

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
            std::print("{} > ", count);
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

        // connect to the server

        return 0;
    }

}  // namespace tcpdb::client
