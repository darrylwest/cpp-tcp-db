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

    const auto start_time = std::chrono::system_clock::now();

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
            using namespace std::chrono_literals;
            using namespace std::chrono;
            auto uptime = duration_cast<seconds>(system_clock::now() - start_time);
            auto oss = base::create_oss();
            oss << "ok, dbsize: " << store.size() << ", uptime: " << uptime;
            oss << ", db file: " << store.get_default_path().string() << '\n';
            return {oss.str()};
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
                    // TODO queue a database append
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

        if (request.starts_with("last")) {
            auto oss = base::create_oss();
            if (auto cmd = base::parse_command(request)) {
                try {
                    const auto count = tcpdb::base::to_number<size_t>(cmd->key.value());
                    auto mp = store.last(count);
                    for (const auto& [k, v] : mp) {
                        oss << k << " " << v << '\n';
                    }
                } catch (const std::exception& e) {
                    spdlog::error("could not parse last command: {}", e.what());
                    return {"bad request, could not parse last command", 401};
                }
            }

            return {oss.str()};
        }

        if (request == "save") {
            const auto& store_path = store.get_default_path();
            spdlog::info("writing database: {}", store_path.string());
            if (store.write(store_path)) {
                return {"ok"};
            } else {
                spdlog::error("could not write database: {}", store_path.string());
                return {"could not write database file: " + store_path.string(), 500};
            }
        }

        if (request.starts_with("txkey")) {
            return {domainkeys::keys::create_timestamp_key().to_string()};
        }

        if (request.starts_with("rtkey")) {
            return {domainkeys::keys::create_route_key().to_string()};
        }

        // TODO now trap for database requests
        if (request.starts_with("dbsize")) {
            return {fmt::format("{}", store.size())};
        }

        if (request.starts_with("keys")) {
            return {base::join(store.keys())};
        }

        if (request == "cleardb") {
            for (auto& key : store.keys()) {
                store.remove(key);
            }
            return {"ok"};
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

        char buf[config::BUFFER_SIZE] = {0};
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

    // set the store's path here
    void set_store_path(const std::string& path) { store.set_default_path(path); }

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

        // TODO read the database
        spdlog::info("reading database: {}", config.server.data_file);
        if (store.read(config.server.data_file)) {
            store.set_default_path(config.server.data_file);
            spdlog::info("database read: {}", store.size());
        } else {
            spdlog::error("could not read database from: {}", config.server.data_file);
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

        spdlog::info("writing database: {}, size: {}", config.server.data_file, store.size());
        auto result = store.write(config.server.data_file);
        spdlog::info("database write result: {}", result ? "ok" : "failed");

        // save the database?

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
