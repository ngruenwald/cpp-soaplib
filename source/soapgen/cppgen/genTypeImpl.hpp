#pragma once

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {
namespace type {

void GenerateImplementation(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition);

} // namespace type
} // namespace cppgen
