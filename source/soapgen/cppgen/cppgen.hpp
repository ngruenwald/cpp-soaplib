#pragma once

#include <string>
#include <wsdl.hpp>

#include "options.hpp"

namespace cppgen {

void Generate(
    Options& options,
    Definition& definition);

} // namespace cppgen
