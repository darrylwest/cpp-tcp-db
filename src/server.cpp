//
// dpw
//

#include <spdlog/spdlog.h>
#include <atomic>
#include <thread>
#include <sockpp/tcp_acceptor.h>
#include <sockpp/version.h>

#include <domainkeys/keys.hpp>
#include <quickkv/quickkv.hpp>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>

namespace tcpdb::server {

    std::atomic_flag halt_threads = ATOMIC_FLAG_INIT;

    quickkv::KVStore store;

    void handle_client(sockpp::tcp_socket sock) {
        char buf[512] = {0};
        sockpp::result<size_t> res;

        while ((res = sock.read(buf, sizeof(buf)) && res.value() > 0)) {
            sock.write_n(buf, res.value());
        }

        sock.write(": ok\n", 4);
        spdlog::info("close socket connection: {}", sock.peer_address().to_string());
        sock.close();
    }

    int start(const config::Config& config) {

        spdlog::info("Starting server: {}", config.to_string());
        spdlog::info("sockpp version: {}", sockpp::SOCKPP_VERSION);

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

}


