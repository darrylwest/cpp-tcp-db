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

    std::atomic_flag halt_threads;

    quickkv::KVStore store;

    int start(const config::Config& config) {

        spdlog::info("Starting server: {}", config.to_string());
        spdlog::info("sockpp version: {}", sockpp::SOCKPP_VERSION);

        halt_threads.clear();

        return 0;
    }

    void shutdown() {
        // TODO save the database

        halt_threads.test_and_set();
    }

}


