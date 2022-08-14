#pragma once

#include <string>
#include <vector>

namespace cppgen {

struct Options
{
    std::string name = "service";

    std::string outputPath = "_output";
    bool appendNamespacesToPath = true;
    std::string typesSubfolder = "types";
    std::vector<std::string> namespaces;

    std::string cmakeNamespace;
    std::string cmakeExport;

    std::vector<std::string> pointerTypes;

    std::string innerTypeSuffix = "T";

    bool writeTimestamp = true;

    bool enableHacks = false;
    std::vector<std::string> ignoredOperations;
    std::vector<std::string> ignoredMessages;
    std::vector<std::string> ignoredTypes;
};

} // namespace cppgen
