//
// 2024-12-09 10:28:16 dpw
//

#include <sstream>
#include <string>
#include <tcpdb/base.hpp>
#include <tcpdb/version.hpp>
#include <thread>
#include <vector>

namespace tcpdb::base {
    namespace screen {
        // constexpr auto red = "\033[0;31m";
        constexpr auto green = "\033[0;32m";
        // constexpr auto yellow = "\033[0;33m";
        // constexpr auto blue = "\033[0;34m";
        // constexpr auto magenta = "\033[0;35m";
        constexpr auto cyan = "\033[0;36m";
        // constexpr auto white = "\033[0;37m";
        // constexpr auto reset = "\033[0m";
        constexpr auto reset_nl = "\033[0m\n";
    }  // namespace screen

    // remove all tne newline character returns
    void remove_newlines(std::string& str) {
        std::erase_if(str, [](char c) { return c == '\n' || c == '\r'; });
    }

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs) {
        std::ostringstream oss;

        for (size_t i = 0; i < strs.size(); ++i) {
            if (i > 0) oss << ",";
            oss << strs[i];
        }

        return oss.str();
    }

    std::string help_text() {
        using namespace screen;
        std::ostringstream oss;
        auto version = tcpdb::Version();

        oss << green << "tcp-db, version: " << version << reset_nl;
        oss << cyan << "Command List\n";
        oss << "  get key       : returns the value from the key, or not found\n";
        oss << "  set key value : writes the value to the database using the key\n";
        oss << "  remove key    : removes the value from the database using the key\n";
        oss << "  keys          : returns all the database keys\n";
        oss << "  dbsize        : returns the number of keys in the database\n";
        oss << "  version       : returns the current version\n";
        oss << "  ping          : pong\n";
        oss << "  status        : returns the server status, uptime, db size, etc.\n";
        oss << "  quit          : ends the socket session\n";
        oss << "  shutdown      : kills the server\n";
        oss << "  help          : shows this help text\n" << reset_nl;


        return oss.str();
    }
}  // namespace tcpdb::base
