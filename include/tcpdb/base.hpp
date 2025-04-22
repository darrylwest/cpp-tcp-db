//
// 2024-12-09 10:28:16 dpw
//

#pragma once

#include <charconv>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace tcpdb::base {

    // to support database command ops
    struct Command {
        std::string name;
        std::optional<std::string> key;
        std::optional<std::string> value;
    };

    // create a concat pairs function that returns a lambda
    auto create_cat_pair(const std::string& separator);

    // parse the command
    std::optional<Command> parse_command(const std::string& input);

    // remove all tne newline character returns
    void remove_newlines(std::string& str);

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs, const std::string& separator = ",");

    // return the long help string with all commands
    const std::string help_text();

    // create a new empty ostringstream
    const auto create_oss = []() { return std::ostringstream(); };

    // convert the int, long, size_t, etc from string to T
    template <typename T> T to_number(const std::string& str) {
        T val;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), val);
        if (ec != std::errc()) {
            throw std::invalid_argument("Conversion failed");
        }
        return val;
    }

}  // namespace tcpdb::base
