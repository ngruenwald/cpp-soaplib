#pragma once

#include <ostream>
#include "options.hpp"
#include "../wsdl.hpp"

namespace cppgen {
namespace service {

void GenerateServerImplementation(
    std::ostream& stream,
    const Service& service,
    const Options& options,
    const Definition& definition);

} // namespace service
} // namespace cppgen
