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
        bright = 60  // Not a standard attribute, but a logical addition.
    };

    // Convert color to ANSI code
    std::string to_string(Color c);

    // convert the attribute to a ANSI code string
    std::string to_string(Attr a);

    // Color Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Color c);

    // Attr Overload << for easier use
    std::ostream& operator<<(std::ostream& os, Attr a);

}  // namespace tcpdb::termio
