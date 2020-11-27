#pragma once

#include <string>
#include "../wsdl.hpp"

namespace cppgen {

bool IsNativeType(
    const Name& name);

std::string ResolveType(
    const Name& name,
    bool stripNamespace = false);

std::string FormatParameterName(
    const std::string& name);

};
