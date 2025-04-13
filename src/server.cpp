//
// dpw
//

#include <sockpp/tcp_acceptor.h>
#include <sockpp/version.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <atomic>
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

    void handle_client(sockpp::tcp_socket sock) {
        spdlog::info("handle client: {}", sock.peer_address().to_string());

        char buf[512] = {0};
        sockpp::result<size_t> res;

        while ((res = sock.read(buf, sizeof(buf))) && res.value() > 0) {
            auto request = std::string(buf, res.value());
            remove_newlines(request);

            spdlog::info("value: {}, buf: {}", res.value(), request);

            if (request.starts_with("version")) {
                auto vers = tcpdb::Version().to_string() + "\n\r";
                sock.write_n(vers.c_str(), vers.size());
            } else if (request == "help") {
                auto msg = std::string("this is the help text.\n\r");
                sock.write_n(msg.c_str(), msg.size());
            } else if (request.starts_with("quit")) {
                spdlog::info("quit requested, closing connections");
                break;
            } else {
                sock.write_n(buf, res.value());
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

        // halt_threads.clear();

        while (!halt_threads.test()) {
            sockpp::inet_address peer;

            if (auto res = acceptor.accept(&peer); !res) {
                spdlog::error("Failed to accept connection: {}", res.error().message());
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
