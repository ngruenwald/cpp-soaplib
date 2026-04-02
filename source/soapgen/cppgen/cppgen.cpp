#include "cppgen.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <functional>
#include <map>
#include <set>

#include "typeMap.hpp"
#include "genTypeHeader.hpp"
#include "genTypeImpl.hpp"
#include "genAnyTypeHeader.hpp"
#include "genAnyTypeImpl.hpp"
#include "genServiceHeader.hpp"
#include "genServiceImpl.hpp"
#include "genServerHeader.hpp"
#include "genServerImpl.hpp"
#include "genCMakeLists.hpp"
#include "utils.hpp"
#include "Renderer.hpp"

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
        const std::string serviceName = safe_name(service.name.name);

        std::cout << "service: " << serviceName << '\n';

        if (options.generateClient)
        {
            const auto headerPath = GetFilePath(serviceName + ".hpp", options, false);
            const auto implPath = GetFilePath(serviceName + ".cpp", options, false);

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

        if (options.generateServer)
        {
            const std::string serverName = serviceName + "Server";
            const auto headerPath = GetFilePath(serverName + ".hpp", options, false);
            const auto implPath = GetFilePath(serverName + ".cpp", options, false);

            std::ofstream hdrFile(headerPath);
            if (!hdrFile.is_open())
            {
                throw std::runtime_error("could not open output file " + headerPath.string());
            }
            std::cout << "  * server hdr:  " << headerPath << std::endl;
            service::GenerateServerHeader(hdrFile, service, options, definition);

            std::ofstream implFile(implPath);
            if (!implFile.is_open())
            {
                throw std::runtime_error("could not open output file " + implPath.string());
            }
            std::cout << "  * server impl: " << implPath << std::endl;
            service::GenerateServerImplementation(implFile, service, options, definition);
        }
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

        auto baseName = ResolveType(type.name, options, true);

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

void GenerateAnyType(
    const Options& options,
    const Definition& definition)
{
    std::string baseName{"SoapLibAnyType"};

    const auto headerPath = GetFilePath(baseName + ".hpp", options, true);
    const auto implPath = GetFilePath(baseName + ".cpp", options, true);

    std::ofstream hdrFile(headerPath);
    if (!hdrFile.is_open())
    {
        throw std::runtime_error("could not open output file " + headerPath.string());
    }
    std::cout << "  * hdr:  " << headerPath << std::endl;
    type::GenerateAnyTypeHeader(hdrFile, options, definition);

    std::ofstream implFile(implPath);
    if (!implFile.is_open())
    {
        throw std::runtime_error("could not open output file " + implPath.string());
    }
    std::cout << "  * impl: " << implPath << std::endl;
    type::GenerateAnyTypeImplementation(implFile, options, definition);
}

//
// CMake
//

void GenerateCMake(
    const Options& options,
    const Definition& definition)
{
    const auto path = GetFilePath("CMakeLists.txt", options, false);

    std::ofstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("could not open output file " + path.string());
    }

    std::cout << "  cmake:  " << path << std::endl;
    cmake::GenerateCMakeLists(file, options, definition);
}

//
//
//

Type* _createNewType(
    const Definition& definition,
    const ExtendedType& type,
    const Parameter& parameter)
{
    const auto it =
        std::find_if(
            std::begin(definition.types),
            std::end(definition.types),
            [&parameter] (const std::shared_ptr<Type>& x) -> bool
            {
                return x ? x->name.name == parameter.type.name : false;
            }
        );

    if (it == std::end(definition.types))
    {
        return {};
    }

    Type* newType = nullptr;

    switch (it->get()->kind)
    {
        case Type::Basic:
            newType = new BasicType{*reinterpret_cast<BasicType*>(it->get())};
            break;
        case Type::Extended:
            newType = new ExtendedType{*reinterpret_cast<ExtendedType*>(it->get())};
            break;
        case Type::Enum:
            newType = new EnumType{*reinterpret_cast<EnumType*>(it->get())};
            break;
    }

    if (!newType)
    {
        return {};
    }

    newType->name.name = type.name.name + "__" + newType->name.name;

    return newType;
}

Type* _createNewType(
    const Definition& definition,
    const ExtendedType& type,
    std::vector<Parameter>& parameters,
    const std::string& parameterName)
{
    // search the parameter
    auto parameter =
        std::find_if(
            std::begin(parameters),
            std::end(parameters),
            [&parameterName] (const Parameter& x) -> bool
            {
                return x.type.name == parameterName;
            }
        );

    if (parameter == std::end(parameters))
    {
        return {};
    }

    // create the new type
    auto newType = _createNewType(definition, type, *parameter);

    // change its type
    parameter->type = newType->name;

    // done
    return newType;
}


void CreateDistinctListTypes(
    Definition& definition)
{
    for (size_t idx = 0; idx < definition.types.size(); ++idx)
    {
        auto typePtr = definition.types[idx];

        if (!typePtr || IsNativeType(typePtr->name) || typePtr->kind != Type::Extended)
        {
            continue;
        }

        auto etype = reinterpret_cast<ExtendedType*>(typePtr.get());

        auto listType = _createNewType(definition, *etype, etype->parameters, "ListDCResponseBase");
        if (!listType)
        {
            continue;
        }
        definition.types.push_back(std::shared_ptr<Type>{listType});

        auto arrayType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(listType)->parameters, "ArrayOfReturnEntity");
        if (!arrayType)
        {
            continue;
        }
        definition.types.push_back(std::shared_ptr<Type>{arrayType});

        auto entityType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(arrayType)->parameters, "ReturnEntity");
        if (!entityType)
        {
            continue;
        }

        auto fn = [] (const Definition& /*definition*/, const ExtendedType& type, std::vector<Parameter>& parameters, const std::string& name) {
            // search the parameter
            auto parameter =
                std::find_if(
                    std::begin(parameters),
                    std::end(parameters),
                    [&name] (const Parameter& x) -> bool
                    {
                        return x.name.name == name;
                    }
                );

            if (parameter != std::end(parameters))
            {
                auto idx = type.name.name.find("_GetListResponse");
                if (idx != std::string::npos)
                {
                    // chante its type
                    parameter->type.name = type.name.name.substr(0, idx) + "DCResponse";
                }
            }
        };

        fn(definition, *etype, reinterpret_cast<ExtendedType*>(entityType)->parameters, "Value");

        definition.types.push_back(std::shared_ptr<Type>{entityType});
    }
}

void RemoveIgnoredTypes(
    const Options& options,
    Definition& definition)
{
    for (const auto& ignored : options.ignoredOperations)
    {
        for (auto& portType : definition.portTypes)
        {
            auto it = std::find_if(
                std::begin(portType.operations),
                std::end(portType.operations),
                [&ignored] (const Operation& op) -> bool
                {
                    return op.name.name == ignored;
                }
            );

            if (it != std::end(portType.operations))
            {
                portType.operations.erase(it);
                std::cout << "XXX removed " << ignored << '\n';
            }
        }
    }

    for (const auto& ignored : options.ignoredMessages)
    {
        auto it = std::find_if(
            std::begin(definition.messages),
            std::end(definition.messages),
            [&ignored] (const Message& msg) -> bool
            {
                return msg.name.name == ignored;
            }
        );

        if (it != std::end(definition.messages))
        {
            definition.messages.erase(it);
            std::cout << "XXX removed " << ignored << '\n';
        }
    }

    for (const auto& ignored : options.ignoredTypes)
    {
        auto it = std::find_if(
            std::begin(definition.types),
            std::end(definition.types),
            [&ignored] (const TypePtr tp) -> bool
            {
                return tp ? tp->name.name == ignored : false;
            }
        );

        if (it != std::end(definition.types))
        {
            definition.types.erase(it);
            std::cout << "XXX removed " << ignored << '\n';
        }
    }
}

//
// Cycles
//

void ResolveCyclicReferences(
    Options& options,
    Definition& definition)
{
    std::map<std::string, TypePtr> typeMap;
    for (const auto& t : definition.types) typeMap[t->name.name] = t;

    std::set<std::string> visited;
    std::set<std::string> stack;

    std::function<void(TypePtr)> visit = [&](TypePtr t) {
        if (!t || t->kind != Type::Extended) return;
        
        const std::string& name = t->name.name;
        if (stack.count(name)) {
            options.cyclicTypes.insert(name);
            return;
        }
        if (visited.count(name)) return;

        visited.insert(name);
        stack.insert(name);

        auto& et = static_cast<ExtendedType&>(*t);
        for (auto& p : et.parameters) {
            if (IsNativeType(p.type)) continue;
            
            auto it = typeMap.find(p.type.name);
            if (it != typeMap.end()) {
                visit(it->second);
                if (options.cyclicTypes.count(p.type.name)) {
                    // Mark this specific parameter as pointer to break the cycle
                    p.kind = Parameter::Pointer;
                }
            }
        }

        stack.erase(name);
    };

    for (const auto& t : definition.types) {
        visit(t);
    }
}

//
// Generate
//

void Generate(
    Options& options,
    Definition& definition)
{
    ResolveCyclicReferences(options, definition);

    // Apply explicit version override if provided
    if (options.soapVersion == Options::SoapVersion::Soap11) {
        definition.version = soaplib::SoapVersion::Soap11;
    } else if (options.soapVersion == Options::SoapVersion::Soap12) {
        definition.version = soaplib::SoapVersion::Soap12;
    }

    if (options.templatePath.empty())
    {
        options.templatePath = "templates";
    }

    Renderer renderer(options.templatePath, options.outputPath);

    if (options.enableHacks)
    {
        // remove stuff that doesn't work
        RemoveIgnoredTypes(options, definition);

        // create types for the dynamic list crap
        CreateDistinctListTypes(definition);

        // remove again... maybe we added something that we don't want ;)
        RemoveIgnoredTypes(options, definition);
    }

    GenerateFolders(options);

    GenerateServices(options, definition);

    GenerateTypes(options, definition);

    GenerateAnyType(options, definition);

    GenerateCMake(options, definition);
}

} // namespace cppgen
