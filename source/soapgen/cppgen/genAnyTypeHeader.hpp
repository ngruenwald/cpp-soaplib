#pragma once

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {
namespace type {

void GenerateAnyTypeHeader(
    std::ostream& stream,
    const Options& options,
    const Definition& definition);

} // namespace type
} // namespace cppgen
