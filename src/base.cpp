//
// 2024-12-09 10:28:16 dpw
//

#include <algorithm>
#include <functional>
#include <optional>
#include <ranges>
#include <sstream>
#include <string>
#include <tcpdb/base.hpp>
#include <tcpdb/version.hpp>
#include <termio/termio.hpp>

namespace tcpdb::base {

    auto create_cat_pair(const std::string& separator) {
        return [separator](const std::string& a, const std::string& b) -> std::string {
            return a.empty() ? b : a + separator + b;
        };
    }

    // parse the command
    std::optional<Command> parse_command(const std::string& input) {
        std::istringstream iss(input);
        Command cmd;
        if (!(iss >> cmd.name)) return std::nullopt;

        if (cmd.name == "get" || cmd.name == "remove" || cmd.name == "write") {
            if (!(iss >> cmd.key.emplace())) return std::nullopt;
        } else if (cmd.name == "set") {
            std::string key;
            if (!(iss >> key)) return std::nullopt;
            cmd.key = key;

            // Read the rest of the line as the value (may contain spaces)
            std::string value;
            std::getline(iss >> std::ws, value);  // consume leading whitespace before getline
            cmd.value = value;
        } else {
            return std::nullopt;  // unknown command
        }

        return cmd;
    }

    // remove all tne newline character returns
    void remove_newlines(std::string& str) {
        std::erase_if(str, [](char c) { return c == '\n' || c == '\r'; });
    }

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs, const std::string& separator) {
        auto cat_pair = create_cat_pair(separator);

        return std::ranges::fold_left(strs.begin(), strs.end(), "", cat_pair);
    }

    const std::string help_text() {
        using namespace termio::termio;

        std::ostringstream oss;
        auto version = tcpdb::Version();

        oss << green() << "tcp-db, version: " << version << reset_nl();
        oss << cyan() << "Command List\n" << yellow();
        oss << "  get key       : returns the value from the key, or not found\n";
        oss << "  set key value : writes the value to the database using the key\n";
        oss << "  remove key    : removes the value from the database using the key\n";
        oss << "  keys          : returns all the database keys\n";
        oss << "  last n        : returns the last n elements of database\n";
        oss << "  search term   : searches values for matches\n";
        oss << "  dbsize        : returns the number of keys in the database\n";
        oss << "  txkey         : create and return a new timestamp key\n";
        oss << "  write         : write k/v store to file name from configuration\n";
        oss << "  read          : read data file into k/v store,\n";
        oss << "  version       : returns the current version\n";
        oss << "  ping          : pong\n";
        oss << "  status        : returns the server status, uptime, db size, etc.\n";
        oss << "  quit          : ends the socket session\n";
        oss << "  shutdown      : kills the server\n";
        oss << "  help          : shows this help text\n" << reset_nl();

        return oss.str();
    }
}  // namespace tcpdb::base
