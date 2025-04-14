//
// 2025-04-14 14:25:31 dpw
//

#pragma once

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace tcpdb::termio {

    // Enum for basic colors
    enum class Color {
        black = 0,
        red = 1,
        green = 2,
        yellow = 3,
        blue = 4,
        magenta = 5,
        cyan = 6,
        white = 7,
        reset = 9
    };

    // Enum for text attributes
    enum class Attr {
        normal = 0,
        bold = 1,
        faint = 2,
        italic = 3,
        underline = 4,
        blink = 5,
        rapid_blink = 6,
        reverse = 7,
        hidden = 8,
        strikethrough = 9,
        // bright = 60  // Not a standard attribute, but a logical addition.
    };

    // Convert color to ANSI code
    std::string to_string(Color c);

    // convert the attribute to a ANSI code string
    std::string to_string(Attr a);

    // Color Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Color c);

    // Attr Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Attr a);

    const std::string wrap(const std::string& s, const Color& c, const Attr& a = Attr::normal);
    const std::string normal();
    const std::string bold();
    const std::string italic();
    const std::string underline();

    const std::string black();
    const std::string red();
    const std::string green();
    const std::string yellow();
    const std::string blue();
    const std::string magenta();
    const std::string cyan();
    const std::string white();
    const std::string reset();
    const std::string reset_nl();

}  // namespace tcpdb::termio
