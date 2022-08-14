#include "utils.hpp"

#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>

#include "options.hpp"
#include "wsdl.hpp"

namespace cppgen {

std::string now()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

std::string safe_name(
    const std::string& input)
{
    const char* bad_chars = "-:.#";
    const char good_char = '_';

    auto output = input;

    std::string::size_type pos = 0;
    while ((pos = output.find_first_of(bad_chars, pos)) != std::string::npos)
    {
        output[pos] = good_char;
        pos += 1;
    }

    return output;
}

bool isPortAllowed(
    const Options& options,
    const Port& port)
{
    if (options.portFilter.empty())
    {
        return true;
    }

    return
        std::find(
            std::begin(options.portFilter),
            std::end(options.portFilter),
            port.name.name
        ) != std::end(options.portFilter);
}

} // namespace cppgen
