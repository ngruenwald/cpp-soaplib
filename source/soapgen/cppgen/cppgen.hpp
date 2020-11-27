#pragma once

#include <string>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {

void Generate(
    const Options& options,
    /*const*/ Definition& definition);

} // namespace cppgen
