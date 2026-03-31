#pragma once

#include <string>
#include "../wsdl.hpp"

#include "options.hpp"

namespace cppgen {

bool IsNativeType(
    const Name& name);

std::string ResolveType(
    const Name& name,
    const Options& options,
    bool stripNamespace = false);

std::string FormatParameterName(
    const std::string& name);

};
