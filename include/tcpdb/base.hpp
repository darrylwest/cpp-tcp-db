//
// 2024-12-09 10:28:16 dpw
//

#pragma once

#include <string>
#include <vector>

namespace tcpdb::base {
    namespace screen {
        constexpr auto red = "\033[0;31m";
        constexpr auto green = "\033[0;32m";
        constexpr auto yellow = "\033[0;33m";
        constexpr auto blue = "\033[0;34m";
        constexpr auto magenta = "\033[0;35m";
        constexpr auto cyan = "\033[0;36m";
        constexpr auto white = "\033[0;37m";
        constexpr auto reset = "\033[0m";
        constexpr auto reset_nl = "\033[0m\n";
    }  // namespace screen

    // remove all tne newline character returns
    void remove_newlines(std::string& str);

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs);

    // return the long help string with all commands
    std::string help_text();

}  // namespace tcpdb::base
