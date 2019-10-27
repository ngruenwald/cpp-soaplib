#include "genTypeHeader.hpp"

#include "typeMap.hpp"
#include "utils.hpp"

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

    stream << indent << "struct " << typePrefix << type.name.name << typeSuffix << '\n';

    if (type.base.has_value())
    {
        stream << indent << "    : " << ResolveType(type.base.value()) << '\n';
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
            "T"
        );
    }
    for (const auto& parameter : type.parameters)
    {
        auto parameterType = ResolveType(parameter.type);

        if (!IsNativeType(parameter.type))
        {
            parameterType = typePrefix + parameterType + typeSuffix;
        }

        if (IsInnerType(parameter.type, type.innerTypes))
        {
            parameterType += "T";
        }

        switch (parameter.kind)
        {
            case Parameter::Mandatory:
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
                    << parameter.name.name
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
                    << parameter.name.name
                    << ";" << '\n';
                break;
            }

            case Parameter::Optional:
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
                    << parameter.name.name
                    << ";" << '\n';
                break;
            }

            case Parameter::Multiple:
            {
                stream
                    << indent
                    << "    "
                    << "std::vector<std::shared_ptr<";

                if (!IsNativeType(parameter.type) &&
                    !IsInnerType(parameter.type, type.innerTypes))
                {
                    stream << nspref;
                }

                stream
                    << parameterType << ">>"
                    << " "
                    << parameter.name.name
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

    stream << indent << "struct " << type.name.name << '\n';
    if (type.base.has_value())
    {
        stream
            << indent
            << "    : "
            << typePrefix << ResolveType(type.base.value()) << typeSuffix
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

    stream
        << indent
        << "enum class "
        << typePrefix << type.name.name << typeSuffix
        << '\n';

    stream << indent << "{" << '\n';

    for (const auto& enumeration : type.enumerations)
    {
        stream << indent << "    " << enumeration.text << ',' << '\n';
    }

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

        const auto parameterType = ResolveType(parameter.type);

        if (parameter.kind == Parameter::Mandatory || parameter.kind == Parameter::Optional)
        {
            continue;
        }

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

        const auto parameterType = ResolveType(parameter.type);

        if (parameter.kind != Parameter::Mandatory && parameter.kind != Parameter::Optional)
        {
            continue;
        }

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
    stream << "// " << type.name.name << '\n';
    stream << "// " << now() << '\n';
    stream << '\n';
    stream << "#pragma once" << '\n';
    stream << '\n';
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
            << /*definition.name.name + "_" +*/ baseType.value().name + ".hpp"
            << "\""
            << '\n';
        stream << '\n';
    }

    GenerateIncludes(stream, type, options, definition);

    stream << "namespace xml { class Node; }" << '\n';
    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    GenerateForwardDeclarations(stream, type, options, definition);

    stream << '\n';

    GenerateTypeHeaderT(stream, 0, type, options, definition, {}, {});

    stream << '\n';

    stream << "void " << type.name.name << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode," << '\n';
    stream << "    " << ResolveType(type.name) << "& obj);" << '\n';

    stream << '\n';

    stream << ResolveType(type.name) << " " << type.name.name << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode);" << '\n';

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}


} // namespace type
} // namespace cppgen
