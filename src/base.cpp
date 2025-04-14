//
// 2024-12-09 10:28:16 dpw
//

#include <sstream>
#include <string>
#include <tcpdb/base.hpp>
#include <tcpdb/termio.hpp>
#include <tcpdb/version.hpp>
#include <thread>
#include <vector>

namespace tcpdb::base {

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
        auto green = termio::to_string(termio::Color::green);
        auto reset = to_string(termio::Color::reset);
        auto cyan = termio::to_string(termio::Color::cyan);
        auto yellow = termio::to_string(termio::Color::yellow);

        std::ostringstream oss;
        auto version = tcpdb::Version();

        oss << green << "tcp-db, version: " << version << reset << '\n';
        oss << cyan << "Command List\n" << yellow;
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
        oss << "  help          : shows this help text\n" << reset << '\n';

        return oss.str();
    }
}  // namespace tcpdb::base
