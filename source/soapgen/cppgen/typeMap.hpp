#pragma once

#include <string>
#include "../wsdl.hpp"

namespace cppgen {

void SetApplyHacks(
    bool applyHacks);

bool IsNativeType(
    const Name& name);

std::string ResolveType(
    const Name& name,
    bool stripNamespace = false);

std::string ResolveParamType(
    const std::string& paramName,
    const Name& name,
    bool stripNamespace = false);

std::string FormatParameterName(
    const std::string& name);

};
