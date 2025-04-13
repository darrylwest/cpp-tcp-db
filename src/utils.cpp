//
// 2024-12-09 10:28:16 dpw
//

#include <tcpdb/utils.hpp>

#include <string>
#include <vector>

namespace tcpdb::helpers {
    // remove all tne newline character returns
    void remove_newlines(std::string& str);

    // join the string array into a single comma delimited string
    std::string join(const std::vector<std::string>& strs);

}
