#include "genTypeImpl.hpp"

#include <iostream>

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

static void GenerateParseEnum(
    std::ostream& stream,
    const EnumType& type,
    const Options& options,
    const Definition& definition)
{

}

static void GenerateParseBasic(
    std::ostream& stream,
    const BasicType& type,
    const Options& options,
    const Definition& definition)
{
    if (type.base.has_value())
    {
        stream
            << "    "
            << ResolveType(type.base.value(), true)
            << "FromXml(objNode, obj);"
            << '\n';
    }
}

static void GenerateParseExtended(
    std::ostream& stream,
    const ExtendedType& type,
    const Options& options,
    const Definition& definition)
{
    if (type.base.has_value())
    {
        stream
            << "    "
            << ResolveType(type.base.value(), true)
            << "FromXml(objNode, obj);"
            << '\n';
    }

    for (const auto parameter : type.parameters)
    {
        stream
            << "    "
            << "obj."
            << parameter.name.name
            << " = ";

        switch (parameter.kind)
        {
            case Parameter::Mandatory:
            {
                stream << "soaplib::getMandatory";
                break;
            }

            case Parameter::Pointer:
            {
                stream << "soaplib::getPointer";
                break;
            }

            case Parameter::Optional:
            {
                stream << "soaplib::getOptional";
                break;
            }

            case Parameter::Multiple:
            {
                stream << "soaplib::getMultiple";
                break;
            }

            default:
            {
                std::cerr << "parameter kind " << parameter.kind << " not implemented\n";

                stream << "soaplib::getMandatory";
                break;
            }
        }

        std::string typePrefix = "";
        std::string typeSuffix = "";

        if (IsInnerType(parameter.type, type.innerTypes))
        {
            typePrefix += type.name.name + "::";
            typeSuffix += "T";
        }

        stream
            << "<"
            << typePrefix << ResolveType(parameter.type, false) << typeSuffix
            << ">(objNode, \""
            << parameter.name.name
            << "\", "
            << ResolveType(parameter.type, true)
            << "FromXml);"
            << '\n';
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

        if (IsInnerType(parameter.type, extendedType.innerTypes))
        {
            continue;
        }

        const auto parameterType = ResolveType(parameter.type);

        stream << "#include \"" << /*definition.name.name << "_" <<*/ parameterType << ".hpp\"" << '\n';
     }

     stream << '\n';
}

void GenerateParser(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition,
    bool isStatic,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    if (type.kind == Type::Extended)
    {
        const auto& etype = reinterpret_cast<const ExtendedType&>(type);
        for (const auto& innerType : etype.innerTypes)
        {
            GenerateParser(
                stream,
                *innerType,
                options,
                definition,
                true,
                typePrefix + type.name.name + "::",
                "T"
            );
        }
    }

    auto parameterType = typePrefix + ResolveType(type.name) + typeSuffix;

    if (isStatic)
    {
        stream << "static ";
    }

    stream << "void " << type.name.name << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode," << '\n';
    stream << "    " << parameterType << "& obj)" << '\n';
    stream << "{" << '\n';

    switch (type.kind)
    {
        case Type::Enum:
        {
            const auto enumType = reinterpret_cast<const EnumType*>(&type);
            GenerateParseEnum(stream, *enumType, options, definition);
            break;
        }

        case Type::Basic:
        {
            const auto basicType = reinterpret_cast<const BasicType*>(&type);
            GenerateParseBasic(stream, *basicType, options, definition);
            break;
        }

        case Type::Extended:
        {
            const auto extendedType = reinterpret_cast<const ExtendedType*>(&type);
            GenerateParseExtended(stream, *extendedType, options, definition);
            break;
        }

        default:
        {
            std::cerr << "type kind " << type.kind << " not implemented\n";
            break;
        }
    }
    stream << "}" << '\n';
    stream << '\n';

    if (isStatic)
    {
        stream << "static ";
    }

    stream << parameterType << " " << type.name.name << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode)" << '\n';
    stream << "{" << '\n';
    stream << "    " << parameterType << " obj;" << '\n';
    stream << "    " << type.name.name << "FromXml(objNode, obj);" << '\n';
    stream << "    " << "return obj;" << '\n';
    stream << "}" << '\n';
    stream << '\n';
}

void GenerateImplementation(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    stream << "// " << type.name.name << '\n';
    stream << "// " << now() << '\n';
    stream << '\n';
    stream << "#include \"" << /*definition.name.name + "_" +*/ type.name.name << ".hpp\"" << '\n';
    stream << '\n';
    stream << "#include <soaplib/parseHelper.hpp>" << '\n';
    stream << '\n';

    GenerateIncludes(stream, type, options, definition);

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    GenerateParser(stream, type, options, definition, false, {}, {});

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}

} // namespace type
} // namespace cppgen
