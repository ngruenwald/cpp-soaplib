#pragma once

#include <ostream>
#include "options.hpp"
#include "../wsdl.hpp"

namespace cppgen {
namespace service {

void GenerateServerHeader(
    std::ostream& stream,
    const Service& service,
    const Options& options,
    const Definition& definition);

} // namespace service
} // namespace cppgen
