//
// 2024-12-09 10:28:16 dpw
//

#pragma once

#include <string>
#include <vector>

namespace tcpdb::base {

    // remove all tne newline character returns
    void remove_newlines(std::string& str);

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs);

    // return the long help string with all commands
    std::string help_text();

}  // namespace tcpdb::base
