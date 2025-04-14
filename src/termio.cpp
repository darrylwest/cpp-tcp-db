//
// 2025-04-14 14:25:31 dpw
//

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tcpdb/termio.hpp>
#include <vector>

namespace tcpdb::termio {

    // Convert color to ANSI code
    std::string to_string(Color c) {
        switch (c) {
            case Color::black:
                return "\033[30m";
            case Color::red:
                return "\033[31m";
            case Color::green:
                return "\033[32m";
            case Color::yellow:
                return "\033[33m";
            case Color::blue:
                return "\033[34m";
            case Color::magenta:
                return "\033[35m";
            case Color::cyan:
                return "\033[36m";
            case Color::white:
                return "\033[37m";
            case Color::reset:
                return "\033[0m";
            default:
                return "\033[0m";  // Reset to default
        }
    }

    std::string to_string(Attr a) {
        switch (a) {
            case Attr::normal:
                return "\033[0m";
            case Attr::bold:
                return "\033[1m";
            case Attr::faint:
                return "\033[2m";
            case Attr::italic:
                return "\033[3m";
            case Attr::underline:
                return "\033[4m";
            case Attr::blink:
                return "\033[5m";
            case Attr::rapid_blink:
                return "\033[6m";
            case Attr::reverse:
                return "\033[7m";
            case Attr::hidden:
                return "\033[8m";
            case Attr::strikethrough:
                return "\033[9m";
            case Attr::bright:
                return ";1";  // Used in conjunction with color.
            default:
                return "\033[0m";
        }
    }

    // Color Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Color c) { return os << to_string(c); }

    // Attr Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Attr a) { return os << to_string(a); }
}  // namespace tcpdb::termio
