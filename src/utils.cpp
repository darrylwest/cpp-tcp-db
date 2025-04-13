//
// 2024-12-09 10:28:16 dpw
//

#include <tcpdb/utils.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

namespace tcpdb::utils {

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
}
