//
// dpw
//

#include <sockpp/tcp_acceptor.h>
#include <sockpp/version.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <domainkeys/keys.hpp>
#include <quickkv/quickkv.hpp>
#include <string>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>
#include <thread>

namespace tcpdb::server {

    std::atomic_flag halt_threads = ATOMIC_FLAG_INIT;

    quickkv::KVStore store;

    // TODO move to helpers.hpp
    void remove_newlines(std::string& str) {
        std::erase_if(str, [](char c) { return c == '\n' || c == '\r'; });
    }

    Response handle_request(const std::string& request) {
        if (request.starts_with("version")) {
            return Response(tcpdb::Version().to_string());
        }

        if (request == "help") {
            // TODO pull this text from helper.hpp
            return Response("help me.");
        }

        if (request.starts_with("ping")) {
            return Response("pong");
        }

        if (request.starts_with("status")) {
            // TODO send the current timestamp, uptime, active connections, db size
            return Response("all a-ok here.");
        }

        // TODO now trap for database requests
        if (request.starts_with("size")) {
            return Response(fmt::format("database size: {}", store.size()));
        }

        if (request.starts_with("quit")) {
            spdlog::info("quit requested, closing connections");
            return Response("quit session", 0, false, true);
        }

        if (request.starts_with("shutdown")) {
            spdlog::info("shutdown requested, closing connections");
            return Response("shutdown requested", 0, true, false);
        }

        // echo back with error set
        auto text = fmt::format("Unknown request: {}", request);
        return Response(text, 401);
    }

    void handle_client(sockpp::tcp_socket sock) {
        spdlog::info("handle client: {}", sock.peer_address().to_string());

        char buf[512] = {0};
        sockpp::result<size_t> res;

        while ((res = sock.read(buf, sizeof(buf))) && res.value() > 0) {
            auto request = std::string(buf, res.value());
            remove_newlines(request);

            spdlog::info("value: {}, buf: {}", res.value(), request);

            auto response = handle_request(request);

            // add the line terminator
            auto data = response.text + "\n\r";

            // write the complete response
            sock.write_n(data.data(), data.size());

            if (response.quit) {
                spdlog::info("quit requested, closing connection");
                break;
            }

            if (response.shutdown) {
                spdlog::info("shutdown requested, closing connections, stopping server");
                halt_threads.test_and_set();
                break;
            }

            // clear the used section of buffer
            std::memset(buf, 0, res.value());
        }

        sock.write("ok\n\r", 4);
        spdlog::info("close socket connection: {}", sock.peer_address().to_string());
        sock.close();
    }

    int start(const config::Config& config) {
        spdlog::info("Starting server: {}", config.to_string());
        spdlog::info("SockPP version: {}", sockpp::SOCKPP_VERSION);

        sockpp::initialize();

        auto port = config.server.port;
        std::error_code ec;
        sockpp::tcp_acceptor acceptor(port, 4, ec);

        if (ec) {
            spdlog::error("Failed to start server: {}", ec.message());
            return 1;
        }

        // struct timeval timeout {config.server.timeout_seconds, 0};
        // spdlog::info("timeout set to {} seconds", config.server.timeout_seconds);

        // acceptor.set_non_blocking(true);

        while (!halt_threads.test()) {
            sockpp::inet_address peer;

            if (auto res = acceptor.accept(&peer); !res) {
                if (res.error().value() != 35) {
                    spdlog::error("Failed to accept connection: {}", res.error().message());
                    spdlog::error("error code: {}", res.error().value());
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            } else {
                spdlog::info("Accepted connection: {}", peer.to_string());
                sockpp::tcp_socket sock = res.release();

                std::thread t(handle_client, std::move(sock));
                t.detach();
            }
        }

        return 0;
    }

    void shutdown() {
        // TODO save the database

        spdlog::info("Shutting down server");

        halt_threads.test_and_set();
    }

}  // namespace tcpdb::server
