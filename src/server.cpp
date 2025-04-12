//
// dpw
//

#include <spdlog/spdlog.h>

#include <domainkeys/keys.hpp>
#include <quickkv/quickkv.hpp>
#include <tcpdb/server.hpp>
#include <tcpdb/version.hpp>

int main() {
    const auto vers = tcpdb::Version();
    spdlog::info("tcp-db server application, version: {}", vers.to_string());

    // create the k/v store (TODO move to server)
    quickkv::KVStore store;

    // create a unique timestamp key
    auto key = domainkeys::keys::create_timestamp_key();
    spdlog::info("key: {}", key.to_string());

    // write to the store
    store.set(key.to_string(), "hello tiny kv store.");

    // show the store size
    spdlog::info("store size: {}", store.size());

    return 0;
}
