#pragma once

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {
namespace cmake {

void GenerateCMakeLists(
    std::ostream& stream,
    const Options& options,
    const Definition& definition);

} // namespace cmake
} // namespace cppgen
