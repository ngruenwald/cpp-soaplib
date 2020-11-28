#include "cppgen.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "typeMap.hpp"
#include "genTypeHeader.hpp"
#include "genTypeImpl.hpp"
#include "genAnyTypeHeader.hpp"
#include "genAnyTypeImpl.hpp"
#include "genServiceHeader.hpp"
#include "genServiceImpl.hpp"
#include "genCMakeLists.hpp"

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

        //std::string baseName = /*definition.name.name + "_" +*/ type.name.name;
        auto baseName = ResolveType(type.name, true);

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

//std::shared_ptr<Type> _createNewType(
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

#if 1
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

    //return std::shared_ptr<Type>(newType);
    return newType;
#else
    auto newType = it->get()->Clone();

    if (!newType)
    {
        return {};
    }

    newType->name.name = type.name.name + "__" + newType->name.name;

    return newType;
#endif
}

//std::shared_ptr<Type> _createNewType(
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
    // TODO: there's a mess with the smartpoints. Fix it.

    // create types for all occurences of
    //   ListDCResponseBase, ArrayOfReturnEntity and ReturnEntity
    // and replace the original types with the generated ones

    std::vector<TypePtr> newTypes;

    //for (auto& typePtr : definition.types)
    for (size_t idx = 0; idx < definition.types.size(); ++idx)
    {
        auto typePtr = definition.types[idx];

        if (!typePtr)
        {
            continue;
        }

        auto& type = *typePtr;

        if (IsNativeType(type.name))
        {
            continue;
        }

        if (type.kind != Type::Extended)
        {
            continue;
        }

        // // testing only
        // if (type.name.name != "FileMetadata_GetListResponse")
        // {
        //     continue;
        // }

        auto etype = reinterpret_cast<ExtendedType*>(&type);

        auto listType = _createNewType(definition, *etype, etype->parameters, "ListDCResponseBase");
        if (!listType)
        {
            continue;
        }
        definition.types.push_back(std::shared_ptr<Type>{listType});

//        auto arrayType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(&listType)->parameters, "ArrayOfReturnEntity");
        auto arrayType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(listType)->parameters, "ArrayOfReturnEntity");
        if (!arrayType)
        {
            continue;
        }
        definition.types.push_back(std::shared_ptr<Type>{arrayType});

//        auto entityType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(&arrayType)->parameters, "ReturnEntity");
        auto entityType = _createNewType(definition, *etype, reinterpret_cast<ExtendedType*>(arrayType)->parameters, "ReturnEntity");
        if (!entityType)
        {
            continue;
        }

        auto fn = [] (const Definition& defintion, const ExtendedType& type, std::vector<Parameter>& parameters, const std::string& name) {
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
//
//

void Generate(
    const Options& options,
    /*const*/ Definition& definition)
{
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
