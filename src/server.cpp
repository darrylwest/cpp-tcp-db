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
#include <tcpdb/base.hpp>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>
#include <thread>

namespace tcpdb::server {

    std::atomic_flag halt_threads = ATOMIC_FLAG_INIT;

    quickkv::KVStore store;

    // handle the client request
    Response handle_request(const std::string& request) {
        if (request.starts_with("version")) {
            return {tcpdb::Version().to_string()};
        }

        if (request == "help") {
            return {base::help_text()};
        }

        if (request.starts_with("ping")) {
            return {"pong"};
        }

        if (request.starts_with("status")) {
            // TODO send the current timestamp, uptime, active connections, db size
            return {"all a-ok here."};
        }

        if (request.starts_with("set ")) {
            if (auto cmd = base::parse_command(request)) {
                auto key = cmd->key.value();
                // if the key is txkey or rtkey then create a key
                if (key == "txkey") {
                    key = domainkeys::keys::create_timestamp_key().to_string();
                    spdlog::info("created txkey: {}", key);
                }

                auto value = cmd->value.value();

                if (store.set(key, value)) {
                    return {"ok"};
                }

                return {"could not set value for key: " + key, 402};
            }

            return {"bad request", 402};
        }

        if (request.starts_with("get ")) {
            if (auto cmd = base::parse_command(request)) {
                auto key = cmd->key.value();

                if (auto value = store.get(key)) {
                    return {value.value()};
                }

                return {"value not found for key: " + key, 404};
            }

            return {"bad request", 402};
        }

        if (request.starts_with("remove ")) {
            if (auto cmd = base::parse_command(request)) {
                auto key = cmd->key.value();

                if (store.remove(key)) {
                    return {"ok"};
                }

                return {"could remove value; not-found key: " + key, 404};
            }

            return {"bad request", 402};
        }

        if (request.starts_with("txkey")) {
            return {domainkeys::keys::create_timestamp_key().to_string()};
        }

        if (request.starts_with("rtkey")) {
            return {domainkeys::keys::create_route_key().to_string()};
        }

        // TODO now trap for database requests
        if (request.starts_with("dbsize")) {
            return {fmt::format("database size: {}", store.size())};
        }

        if (request.starts_with("keys")) {
            if (store.size() == 0) {
                return {"database is empty."};
            }
            return {base::join(store.keys())};
        }

        // the session terminators...
        if (request.starts_with("quit")) {
            spdlog::info("quit requested, closing connections");
            return {"quit session", 0, false, true};
        }

        if (request.starts_with("shutdown")) {
            spdlog::info("shutdown requested, closing connections");
            return {"shutdown requested", 0, true, false};
        }

        // echo back with error set
        auto text = fmt::format("Unknown request: {}", request);
        return {text, 401};
    }

    void handle_client(sockpp::tcp_socket sock) {
        spdlog::info("handle client: {}", sock.peer_address().to_string());

        char buf[512] = {0};
        sockpp::result<size_t> res;

        while ((res = sock.read(buf, sizeof(buf))) && res.value() > 0) {
            auto request = std::string(buf, res.value());
            base::remove_newlines(request);

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

        sock.write_n("ok\n\r", 4);
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

        // shutdown and close all the connections
        acceptor.shutdown();
        acceptor.close();

        return 0;
    }

    void shutdown() {
        // TODO save the database

        spdlog::info("Shutting down server");

        halt_threads.test_and_set();

        // TODO now ping the port to trigger the shutdown
    }

}  // namespace tcpdb::server
