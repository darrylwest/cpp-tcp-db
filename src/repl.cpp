//
// dpw
//

#include <spdlog/spdlog.h>

#include <tcpdb/client.hpp>
#include <termio/termio.hpp>
#include <tcpdb/version.hpp>
#include <print>

const char* BANNER = R"(
___________________________________
   __                       ____
  / /__________        ____/ / /_
 / __/ ___/ __ \______/ __  / __ \
/ /_/ /__/ /_/ /_____/ /_/ / /_/ /
\__/\___/ .___/      \__,_/_.___/
       /_/
___________________________________
)";

int main() {
    using namespace termio::termio;
    const auto vers = tcpdb::Version();
    std::println("{}{}{}", green(), BANNER, reset());
    std::println("{}Client Application, {}Version: {}{}", cyan(), yellow(), vers.to_string(), reset());

    return 0;
}
