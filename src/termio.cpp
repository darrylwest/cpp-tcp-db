//
// 2025-04-14 14:25:31 dpw
//

#include <iostream>
#include <sstream>
#include <string>
#include <tcpdb/termio.hpp>

namespace tcpdb::termio {

    // Convert color to ANSI code
    std::string to_string(Color c, bool bright) {
        switch (c) {
            case Color::black:
                return bright ? "\033[30m" : "\033[1;31m";
            case Color::red:
                return bright ? "\033[31m" : "\033[1;31m";
            case Color::green:
                return bright ? "\033[32m" : "\033[1;32m";
            case Color::yellow:
                return bright ? "\033[33m" : "\033[1;33m";
            case Color::blue:
                return bright ? "\033[34m" : "\033[1;34m";
            case Color::magenta:
                return bright ? "\033[35m" : "\033[1;35m";
            case Color::cyan:
                return bright ? "\033[36m" : "\033[1;36m";
            case Color::white:
                return bright ? "\033[37m" : "\033[1;37m";
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
            // case Attr::bright:
            // return ";1";  // Used in conjunction with color.
            default:
                return "\033[0m";
        }
    }

    // Color Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Color c) { return os << to_string(c); }

    // Attr Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Attr a) { return os << to_string(a); }

    const std::string wrap(const std::string& s, const Color& c, const Attr& a) {
        std::stringstream ss;

        ss << a << c << s << Color::reset;
        return ss.str();
    }

    const std::string normal() { return to_string(Attr::normal); }
    const std::string bold() { return to_string(Attr::bold); }
    const std::string italic() { return to_string(Attr::italic); }
    const std::string underline() { return to_string(Attr::underline); }

    const std::string black() { return to_string(Color::black); }
    const std::string red() { return to_string(Color::red); }
    const std::string green() { return to_string(Color::green); }
    const std::string yellow() { return to_string(Color::yellow); }
    const std::string blue() { return to_string(Color::blue); }
    const std::string magenta() { return to_string(Color::magenta); }
    const std::string cyan() { return to_string(Color::cyan); }
    const std::string white() { return to_string(Color::white); }
    const std::string reset() { return to_string(Color::reset); }
    const std::string reset_nl() { return to_string(Color::reset) + "\n"; }

}  // namespace tcpdb::termio
