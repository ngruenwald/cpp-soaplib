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

static void GenerateParseEnum(
    std::ostream& stream,
    const EnumType& type,
    const Options& options,
    const Definition& definition)
{
    stream << "    auto s = stringFromXml(objNode);" << '\n';

    bool elseIf = false;

    for (const auto& enumeration : type.enumerations)
    {
        stream << "    ";
        if (elseIf)
        {
            stream << "else ";
        }
        stream << "if (s == \"" << enumeration.text << "\")" << '\n';

        stream << "    {" << '\n';
        stream << "        obj = " << ResolveType(type.name) << "::" << enumeration.text << ";" << '\n';
        stream << "    }" << '\n';

        elseIf = true;
    }
}

static void GenerateWriteEnum(
    std::ostream& stream,
    const EnumType& type,
    const Options& options,
    const Definition& definition)
{
    stream << "    std::string s;" << '\n';
    stream << "    switch (obj)" << '\n';
    stream << "    {" << '\n';

    std::string typeName = ResolveType(type.name);

    for (const auto& enumeration : type.enumerations)
    {
        stream << "        case " << typeName << "::" << enumeration.text << ":" << '\n';
        stream << "        {" << '\n';
        stream << "            s = \"" << enumeration.text << "\";" << '\n';
        stream << "            break;" << '\n';
        stream << "        }" << '\n';
    }

    stream << "        default: break;" << '\n';

    stream << "    }" << '\n';
    stream << "    stringToXml(objNode, s);" << '\n';
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

static void GenerateWriteBasic(
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
            << "ToXml(doc, objNode);"
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
        std::string parameterName = FormatParameterName(parameter.name.name);

        stream
            << "    "
            << "obj."
            << parameterName
            << " = ";

        bool usePointer = IsPointerType(parameter.name, options);

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
                if (usePointer)
                {
                    stream << "soaplib::getPointer";
                }
                else
                {
                    stream << "soaplib::getOptional";
                }
                break;
            }

            case Parameter::Multiple:
            {
                if (usePointer)
                {
                    stream << "soaplib::getMultiplePtrs";
                }
                else
                {
                    stream << "soaplib::getMultiple";
                }

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
            typeSuffix += options.innerTypeSuffix;
        }

        stream
            << "<"
            << typePrefix << ResolveParamType(parameter.name.name, parameter.type, false) << typeSuffix
            << ">(objNode, \""
            << parameterName
            << "\", "
            << ResolveParamType(parameter.name.name, parameter.type, true)
            << "FromXml);"
            << '\n';
    }
}

static void GenerateWriteExtended(
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
            << "ToXml(obj, doc, objNode, false);"
            << '\n';
    }

    for (const auto parameter : type.parameters)
    {/*
        auto parameterName = FormatParameterName(parameter.name.name);

        stream
            << "    "
            << "obj."
            << parameterName
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
        }*/

        std::string typePrefix = "";
        std::string typeSuffix = "";

        if (IsInnerType(parameter.type, type.innerTypes))
        {
            typePrefix += type.name.name + "::";
            typeSuffix += options.innerTypeSuffix;
        }
/*
        stream
            << "<"
            << typePrefix << ResolveType(parameter.type, false) << typeSuffix
            << ">(objNode, \""
            << parameterName
            << "\", "
            << ResolveType(parameter.type, true)
            << "FromXml);"
            << '\n';
            */
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

        const auto parameterType = ResolveParamType(parameter.name.name, parameter.type);

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

    auto typeName = ResolveType(type.name);
    auto parameterType = typePrefix + typeName + typeSuffix;

    if (isStatic)
    {
        stream << "static ";
    }

    stream << "void " << typeName << "FromXml(" << '\n';
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

    stream << parameterType << " " << typeName << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode)" << '\n';
    stream << "{" << '\n';
    stream << "    " << parameterType << " obj;" << '\n';
    stream << "    " << typeName << "FromXml(objNode, obj);" << '\n';
    stream << "    " << "return obj;" << '\n';
    stream << "}" << '\n';
    stream << '\n';


    stream << "static void _" << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << parameterType << "& obj," << '\n';
    stream << "    " << "xml::Document& doc," << '\n';
    stream << "    " << "xml::Node& objNode)" << '\n';
    stream << "{" << '\n';

    // TODO
    switch (type.kind)
    {
        case Type::Enum:
        {
            const auto enumType = reinterpret_cast<const EnumType*>(&type);
            GenerateWriteEnum(stream, *enumType, options, definition);
            break;
        }

        case Type::Basic:
        {
            const auto basicType = reinterpret_cast<const BasicType*>(&type);
            GenerateWriteBasic(stream, *basicType, options, definition);
            break;
        }

        case Type::Extended:
        {
            const auto extendedType = reinterpret_cast<const ExtendedType*>(&type);

            for (const auto& parameter : extendedType->parameters)
            {
                auto parameterName = FormatParameterName(parameter.name.name);
                auto parameterNameXml = parameter.name.name;
                bool usePointer = IsPointerType(parameter.name, options);

                switch (parameter.kind)
                {
                    case Parameter::Mandatory:
                    {
                        stream << "    {" << '\n';
                        stream << "        auto pn = soaplib::addChild(doc, objNode, \""
                               << parameterNameXml
                               << "\", \"" << parameter.name.nsHref
                               << "\", \"" << parameter.name.nsPrefix
                               << "\");" << '\n';
                        if (IsNativeType(parameter.type))
                        {
                            stream << "       "
                                   << ResolveType(parameter.type, true)
                                   << "ToXml(pn, obj."
                                   << parameterName
                                   << ");"
                                   << '\n';
                        }
                        else
                        {
                            stream << "        "
                                   << ResolveType(parameter.type, true)
                                   << "ToXml(obj."
                                   << parameterName
                                   << ", doc, pn, false);"
                                   << '\n';
                        }
                        stream << "    }" << '\n';
                        break;
                    }

                    case Parameter::Optional:
                    {
                        if (usePointer)
                        {
                            stream << "    if (obj." << parameterName << ")" << '\n';
                            stream << "    {" << '\n';
                            stream << "        auto pn = soaplib::addChild(doc, objNode, \""
                                   << parameterNameXml
                                   << "\", \"" << parameter.name.nsHref
                                   << "\", \"" << parameter.name.nsPrefix
                                   << "\");" << '\n';
                            if (IsNativeType(parameter.type))
                            {
                                stream << "        " << ResolveType(parameter.type, true)
                                       << "ToXml(pn, *obj." << parameterName << ");"
                                       << '\n';
                            }
                            else
                            {
                                stream << "        " << ResolveType(parameter.type, true)
                                    << "ToXml(*obj."
                                    << parameterName
                                    << ", doc, pn, false);"
                                    << '\n';
                            }
                            stream << "    }" << '\n';
                        }
                        else
                        {
                            stream << "    if (obj." << parameterName << ".has_value())" << '\n';
                            stream << "    {" << '\n';
                            stream << "        auto pn = soaplib::addChild(doc, objNode, \""
                                << parameterNameXml
                                << "\", \"" << parameter.name.nsHref
                                << "\", \"" << parameter.name.nsPrefix
                                << "\");" << '\n';
                            if (IsNativeType(parameter.type))
                            {
                                stream << "        " << ResolveType(parameter.type, true)
                                       << "ToXml(pn, obj." << parameterName << ".value());"
                                       << '\n';
                            }
                            else
                            {
                                stream << "        " << ResolveParamType(parameter.name.name, parameter.type, true)
                                       << "ToXml(obj."
                                       << parameterName
                                       << ".value(), doc, pn, false);"
                                       << '\n';
                            }
                            stream << "    }" << '\n';
                        }
                        break;
                    }

                    case Parameter::Pointer:
                    {
                        stream << "    if (obj." << parameterName << ")" << '\n';
                        stream << "    {" << '\n';
                        stream << "        auto pn = soaplib::addChild(doc, objNode, \""
                               << parameterNameXml
                               << "\", \"" << parameter.name.nsHref
                               << "\", \"" << parameter.name.nsPrefix
                               << "\");" << '\n';
                        if (IsNativeType(parameter.type))
                        {
                            stream << "        " << ResolveType(parameter.type, true)
                                   << "ToXml(pn, *obj." << parameterName << ");"
                                   << '\n';
                        }
                        else
                        {
                            stream << "        " << ResolveParamType(parameter.name.name, parameter.type, true)
                                   << "ToXml(*obj."
                                   << parameterName
                                   << ", doc, pn, false);"
                                   << '\n';
                        }
                        stream << "    }" << '\n';
                        break;
                    }

                    case Parameter::Multiple:
                    {
                        if (usePointer)
                        {
                            stream << "    for (const auto& entry : obj." << parameterName << ")" << '\n';
                            stream << "    {" << '\n';
                            stream << "        if (entry)" << '\n';
                            stream << "        {" << '\n';
                            stream << "            auto pn = soaplib::addChild(doc, objNode, \""
                                << parameterNameXml
                                << "\", \"" << parameter.name.nsHref
                                << "\", \"" << parameter.name.nsPrefix
                                << "\");" << '\n';
                            if (IsNativeType(parameter.type))
                            {
                                stream << "            " << ResolveType(parameter.type, true)
                                       << "ToXml(pn, *entry);"
                                       << '\n';
                            }
                            else
                            {
                                stream << "            " << ResolveParamType(parameter.name.name, parameter.type, true)
                                       << "ToXml(*entry, doc, pn, false);" << '\n';
                            }
                            stream << "        }" << '\n';
                            stream << "    }" << '\n';
                        }
                        else
                        {
                            stream << "    for (const auto& entry : obj." << parameterName << ")" << '\n';
                            stream << "    {" << '\n';
                            stream << "        auto pn = soaplib::addChild(doc, objNode, \""
                                << parameterNameXml
                                << "\", \"" << parameter.name.nsHref
                                << "\", \"" << parameter.name.nsPrefix
                                << "\");" << '\n';
                            if (IsNativeType(parameter.type))
                            {
                                stream << "        " << ResolveType(parameter.type, true)
                                       << "ToXml(pn, entry);"
                                       << '\n';
                            }
                            else
                            {
                                stream << "        " << ResolveParamType(parameter.name.name, parameter.type, true)
                                       << "ToXml(entry, doc, pn, false);" << '\n';
                            }
                            stream << "    }" << '\n';
                        }
                        break;
                    }
                }
            }

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

    stream << "void " << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << parameterType << "& obj," << '\n';
    stream << "    " << "xml::Document& doc," << '\n';
    stream << "    " << "xml::Node& parentNode, " << '\n';
    stream << "    " << "bool createNode)" << '\n';
    stream << "{" << '\n';
    stream << "    if (createNode)" << '\n';
    stream << "    {" << '\n';
    stream << "        "
           << "auto objNode = soaplib::addChild(doc, parentNode, "
           << "\""
           << type.name.name
           << "\", \""
           << type.name.nsHref
           << "\", \""
           << type.name.nsPrefix
           << "\");"
           << '\n';
    stream << "        _" << typeName << "ToXml(obj, doc, objNode);" << '\n';
    stream << "    }" << '\n';
    stream << "    else" << '\n';
    stream << "    {" << '\n';
    stream << "        _" << typeName << "ToXml(obj, doc, parentNode);" << '\n';
    stream << "    }" << '\n';
    stream << "}" << '\n';
    stream << '\n';
}

void GenerateImplementation(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    auto typeName = ResolveType(type.name);

    stream << "// " << typeName << '\n';
    if (options.writeTimestamp)
    {
        stream << "// " << now() << '\n';
    }
    stream << '\n';
    stream << "#include \"" << /*definition.name.name + "_" +*/ typeName << ".hpp\"" << '\n';
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
