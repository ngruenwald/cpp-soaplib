#pragma once

#include <string>

namespace cppgen { struct Options; }
struct Port;

namespace cppgen {

std::string now();

std::string safe_name(
    const std::string& input);

bool isPortAllowed(
    const Options& options,
    const Port& port);

} // namespace cppgen
