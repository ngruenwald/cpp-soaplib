#pragma once

#include <string>
#include <vector>

namespace cppgen {

struct Options
{
    std::string outputPath = "_output";
    bool appendNamespacesToPath = true;
    std::string typesSubfolder = "types";
    std::vector<std::string> namespaces;
    std::string cmakeNamespace;

    std::vector<std::string> pointerTypes;
};

} // namespace cppgen
