#include "genTypeHeader.hpp"

#include "typeMap.hpp"
#include "utils.hpp"

#include <iostream>

namespace cppgen {
namespace type {

static bool IsInnerType(
    const Name& name,
    const std::vector<TypePtr>& innerTypes)
{
    for (const auto& innerType : innerTypes)
    {
        if (name.name == innerType->name.name)
        {
            return true;
        }
    }

    return false;
}

static bool IsPointerType(
    const Name& name,
    const Options& options)
{
    for (const auto& pt : options.pointerTypes)
    {
        if (name.name == pt)
        {
            return true;
        }
    }

    return false;
}

static void GenerateTypeHeaderT(
    std::ostream& stream,
    int indentSize,
    const Type& type,
    const Options& options,
    const Definition& definition,
    const std::string& typePrefix,
    const std::string& typeSuffix);

static void GenerateTypeHeaderET(
    std::ostream& stream,
    int indentSize,
    const ExtendedType& type,
    const Options& options,
    const Definition& definition,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    const std::string indent(indentSize, ' ');

    std::string nspref = "::";
    for (const auto& ns : options.namespaces)
    {
        nspref += ns + "::";
    }

    stream << indent << "struct " << typePrefix << ResolveType(type.name, true) << typeSuffix << '\n';

    if (type.base.has_value())
    {
        stream << indent << "    : " << ResolveType(type.base.value()) << '\n';
    }
    else
    {
        stream << indent << "    : " << "soaplib::SoapBaseType" << '\n';
    }


    stream << indent << "{" << '\n';
    for (const auto& innerType : type.innerTypes)
    {
        GenerateTypeHeaderT(
            stream,
            indentSize + 4,
            *innerType,
            options,
            definition,
            {},
            options.innerTypeSuffix
        );
    }
    for (const auto& parameter : type.parameters)
    {
        auto parameterName = FormatParameterName(parameter.name.name);
        auto parameterType = ResolveType(parameter.type);
        bool usePointer = IsPointerType(parameter.name, options);

        if (!IsNativeType(parameter.type))
        {
            parameterType = typePrefix + parameterType;// + typeSuffix;
        }

        if (IsInnerType(parameter.type, type.innerTypes))
        {
            parameterType += options.innerTypeSuffix;
        }

        switch (parameter.kind)
        {
            case Parameter::Mandatory:
            {
                stream
                    << indent
                    << "    ";

                if (!IsNativeType(parameter.type) &&
                    !IsInnerType(parameter.type, type.innerTypes))
                {
                    stream << nspref;
                }

                stream
                    << parameterType
                    << " "
                    << parameterName
                    << ";" << '\n';
                break;
            }

            case Parameter::Pointer:
            {
                stream
                    << indent
                    << "    "
                    << "std::shared_ptr<";

                if (!IsNativeType(parameter.type) &&
                    !IsInnerType(parameter.type, type.innerTypes))
                {
                    stream << nspref;
                }

                stream
                    << parameterType << ">"
                    << " "
                    << parameterName
                    << ";" << '\n';
                break;
            }

            case Parameter::Optional:
            {
                stream
                    << indent
                    << "    ";

                if (usePointer)
                {
                    stream << "std::shared_ptr<";
                }
                else
                {
                    stream << "std::optional<";
                }

                if (!IsNativeType(parameter.type) &&
                    !IsInnerType(parameter.type, type.innerTypes))
                {
                    stream << nspref;
                }

                stream
                    << parameterType << ">"
                    << " "
                    << parameterName
                    << ";" << '\n';
                break;
            }

            case Parameter::Multiple:
            {
                stream
                    << indent
                    << "    ";

                if (usePointer)
                {
                    stream << "std::vector<std::shared_ptr<";
                }
                else
                {
                    stream << "std::vector<";
                }


                if (!IsNativeType(parameter.type) &&
                    !IsInnerType(parameter.type, type.innerTypes))
                {
                    stream << nspref;
                }

                stream << parameterType;
                if (usePointer)
                {
                    stream << ">>";
                }
                else
                {
                    stream << ">";
                }
                stream
                    << " "
                    << parameterName
                    << ";" << '\n';
                break;
            }
        }
    }
    stream << indent << "};" << '\n';
}

static void GenerateTypeHeaderB(
    std::ostream& stream,
    int indentSize,
    const BasicType& type,
    const Options& options,
    const Definition& definition,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    const std::string indent(indentSize, ' ');

    stream << indent << "struct " << typePrefix << ResolveType(type.name, true) << typeSuffix << '\n';
    if (type.base.has_value())
    {
        stream
            << indent
            << "    : "
            << typePrefix << ResolveType(type.base.value()) << typeSuffix
            << '\n';
    }
    else
    {
        stream
            << indent
            << "    : "
            << typePrefix << "soaplib::SoapBaseType" << typeSuffix
            << '\n';
    }

    stream << indent << "{" << '\n';
    stream << indent << "};" << '\n';
}

static void GenerateTypeHeaderE(
    std::ostream& stream,
    int indentSize,
    const EnumType& type,
    const Options& options,
    const Definition& definition,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    const std::string indent(indentSize, ' ');

    stream << indent << "struct " << typePrefix << ResolveType(type.name, true) << typeSuffix << '\n';
    stream << indent << "    : " << typePrefix << "soaplib::SoapBaseType" << typeSuffix << '\n';
    stream << indent << "{" << '\n';
    stream << indent << indent << "enum Values" << '\n';
    stream << indent << indent << "{" << '\n';
    for (const auto& enumeration : type.enumerations)
    {
        stream << indent << indent << "    " << enumeration.text << ',' << '\n';
    }
    stream << indent << indent << "};" << '\n';
    stream << indent << indent << '\n';
    stream << indent << indent << "Values Value;" << '\n';
    stream << indent << "};" << '\n';
}

static void GenerateTypeHeaderT(
    std::ostream& stream,
    int indentSize,
    const Type& type,
    const Options& options,
    const Definition& definition,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    switch (type.kind)
    {
        case Type::Enum:
        {
            const auto enumType = reinterpret_cast<const EnumType*>(&type);
            GenerateTypeHeaderE(
                stream,
                indentSize,
                *enumType,
                options,
                definition,
                typePrefix,
                typeSuffix
            );
            break;
        }

        case Type::Basic:
        {
            const auto basicType = reinterpret_cast<const BasicType*>(&type);
            GenerateTypeHeaderB(
                stream,
                indentSize,
                *basicType,
                options,
                definition,
                typePrefix,
                typeSuffix
            );
            break;
        }

        case Type::Extended:
        {
            const auto extendedType = reinterpret_cast<const ExtendedType*>(&type);
            GenerateTypeHeaderET(
                stream,
                indentSize,
                *extendedType,
                options,
                definition,
                typePrefix,
                typeSuffix
            );
            break;
        }
    }
}

static void GenerateForwardDeclarations(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    if (type.kind != Type::Extended)
    {
        return;
    }

    const auto& extendedType = reinterpret_cast<const ExtendedType&>(type);

    for (const auto& innerType : extendedType.innerTypes)
    {
        GenerateForwardDeclarations(stream, *innerType, options, definition);
    }

    stream << '\n';

    for (const auto& parameter : extendedType.parameters)
    {
        if (IsNativeType(parameter.type))
        {
            continue;
        }

        if (IsInnerType(parameter.type, extendedType.innerTypes))
        {
            continue;
        }

        if (!IsPointerType(parameter.name, options))
        {
            continue;
        }

        const auto parameterType = ResolveType(parameter.type);

        /*
        if (parameter.kind == Parameter::Mandatory ||
            parameter.kind == Parameter::Optional ||
            parameter.kind == Parameter::Multiple)
        {
            continue;
        }
        */

        stream << "struct " << parameterType << ";" << '\n';
    }
}

static void GenerateIncludes(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    if (type.kind != Type::Extended)
    {
        return;
    }

    const auto& extendedType = reinterpret_cast<const ExtendedType&>(type);

    for (const auto& innerType : extendedType.innerTypes)
    {
        GenerateIncludes(stream, *innerType, options, definition);
    }

    for (const auto& parameter : extendedType.parameters)
    {
        if (IsNativeType(parameter.type))
        {
            continue;
        }

        if (IsPointerType(parameter.name, options))
        {
            continue;
        }

        if (IsInnerType(parameter.type, extendedType.innerTypes))
        {
            continue;
        }

        const auto parameterType = ResolveType(parameter.type);

        /*
        if (parameter.kind != Parameter::Mandatory &&
            parameter.kind != Parameter::Optional &&
            parameter.kind != Parameter::Multiple)
        {
            continue;
        }
        */

        stream << "#include \"" << /*definition.name.name << "_" <<*/ parameterType << ".hpp\"" << '\n';
     }

     stream << '\n';
}

void GenerateHeader(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    auto typeName = ResolveType(type.name, true);

    stream << "// " << typeName << '\n';
    if (options.writeTimestamp)
    {
        stream << "// " << now() << '\n';
    }
    stream << '\n';
    stream << "#pragma once" << '\n';
    stream << '\n';
    stream << "#include <memory>" << '\n';
    stream << "#include <soaplib/basicTypes.hpp>" << '\n';
    stream << '\n';

    std::optional<Name> baseType;

    switch (type.kind)
    {
        case Type::Basic:
        {
            baseType = reinterpret_cast<const BasicType*>(&type)->base;
            break;
        }

        case Type::Extended:
        {
            baseType = reinterpret_cast<const ExtendedType*>(&type)->base;
            break;
        }
    }

    if (baseType.has_value() && !IsNativeType(baseType.value()))
    {
        stream
            << "#include \""
            << /*definition.name.name + "_" +*/ ResolveType(baseType.value(), true) + ".hpp"
            << "\""
            << '\n';
        stream << '\n';
    }

    GenerateIncludes(stream, type, options, definition);

    stream << "namespace soaplib { namespace xml { class Document; } }" << '\n';
    stream << "namespace soaplib { namespace xml { class Node; } }" << '\n';
    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    GenerateForwardDeclarations(stream, type, options, definition);

    stream << '\n';

    GenerateTypeHeaderT(stream, 0, type, options, definition, {}, {});

    stream << '\n';

    stream << "void " << typeName << "FromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& objNode," << '\n';
    stream << "    " << ResolveType(type.name) << "& obj);" << '\n';

    stream << '\n';

    stream << typeName << " " << typeName << "FromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& objNode);" << '\n';

    stream << '\n';

    stream << "std::shared_ptr<soaplib::SoapBaseType> " << typeName << "PtrFromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& objNode);" << '\n';

    stream << '\n';

    stream << "void " << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << typeName << "& obj," << '\n';
    stream << "    " << "soaplib::xml::Document& doc," << '\n';
    stream << "    " << "soaplib::xml::Node& parentNode, " << '\n';
    stream << "    " << "bool createNode);" << '\n';

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}


} // namespace type
} // namespace cppgen
