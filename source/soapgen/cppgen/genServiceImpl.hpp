#pragma once

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {
namespace service {

void GenerateImplementation(
    std::ostream& stream,
    const Service& service,
    const Options& options,
    const Definition& definition);

} // namespace service
} // namespace cppgen
