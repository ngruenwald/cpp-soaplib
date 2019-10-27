#include "cppgen.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "typeMap.hpp"
#include "genTypeHeader.hpp"
#include "genTypeImpl.hpp"
#include "genServiceHeader.hpp"
#include "genServiceImpl.hpp"

namespace cppgen {

//
// Files
//

std::filesystem::path GetBasePath(
    const Options& options,
    bool includeType)
{
    std::filesystem::path path{options.outputPath};

    if (options.appendNamespacesToPath)
    {
        for (const auto& ns : options.namespaces)
        {
            path.append(ns);
        }
    }

    if (includeType && !options.typesSubfolder.empty())
    {
        path.append(options.typesSubfolder);
    }

    return path;
}

std::filesystem::path GetFilePath(
    const std::string& fileName,
    const Options& options,
    bool isType)
{
    auto path = GetBasePath(options, isType);
    path.append(fileName);
    return path;
}

void GenerateFolders(
    const Options& options)
{
    const auto path = GetBasePath(options, true);
    std::filesystem::create_directories(path);
}

//
// Service
//

void GenerateServices(
    const Options& options,
    const Definition& definition)
{
    for (const auto& service : definition.services)
    {
        std::cout << "service: " << service.name.name << '\n';

        const auto headerPath = GetFilePath(service.name.name + ".hpp", options, false);
        const auto implPath = GetFilePath(service.name.name + ".cpp", options, false);

        std::ofstream hdrFile(headerPath);
        if (!hdrFile.is_open())
        {
            throw std::runtime_error("could not open output file " + headerPath.string());
        }
        std::cout << "  * hdr:  " << headerPath << std::endl;
        service::GenerateHeader(hdrFile, service, options, definition);

        std::ofstream implFile(implPath);
        if (!implFile.is_open())
        {
            throw std::runtime_error("could not open output file " + implPath.string());
        }
        std::cout << "  * impl: " << implPath << std::endl;
        service::GenerateImplementation(implFile, service, options, definition);
    }
}

//
// Types
//

void GenerateTypes(
    const Options& options,
    const Definition& definition)
{
    for (const auto& typePtr : definition.types)
    {
        if (!typePtr)
        {
            continue;
        }

        const auto& type = *typePtr;

        std::cout << "type: " << type.name.name << '\n';

        if (IsNativeType(type.name))
        {
            std::cout << "  * native" << std::endl;
            continue;
        }

        std::string baseName = /*definition.name.name + "_" +*/ type.name.name;

        const auto headerPath = GetFilePath(baseName + ".hpp", options, true);
        const auto implPath = GetFilePath(baseName + ".cpp", options, true);

        std::ofstream hdrFile(headerPath);
        if (!hdrFile.is_open())
        {
            throw std::runtime_error("could not open output file " + headerPath.string());
        }
        std::cout << "  * hdr:  " << headerPath << std::endl;
        type::GenerateHeader(hdrFile, type, options, definition);

        std::ofstream implFile(implPath);
        if (!implFile.is_open())
        {
            throw std::runtime_error("could not open output file " + implPath.string());
        }
        std::cout << "  * impl: " << implPath << std::endl;
        type::GenerateImplementation(implFile, type, options, definition);
    }
}

void Generate(
    const Options& options,
    const Definition& definition)
{
    GenerateFolders(options);

    GenerateServices(options, definition);

    GenerateTypes(options, definition);
}

} // namespace cppgen
