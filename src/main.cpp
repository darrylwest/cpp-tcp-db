//
// dpw
//

#include <spdlog/spdlog.h>

#include <tcpdb/tiny.hpp>
#include <tcpdb/version.hpp>
#include <domainkeys/keys.hpp>
#include <quickkv/quickkv.hpp>

int main() {
    const auto vers = app::Version();
    constexpr auto lang = "c++";
    spdlog::info("Hello and welcome to {} tiny application, version: {}", lang, vers.to_string());

    // create the k/v store
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
