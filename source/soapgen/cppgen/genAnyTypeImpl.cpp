#include "genTypeImpl.hpp"

#include <iostream>

#include "typeMap.hpp"
#include "utils.hpp"

namespace cppgen {
namespace type {

static void GenerateParseBasic(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    stream << "    auto type = objNode.GetStringProp(\"type\");" << '\n';
    stream << "    auto idx = type.find(\':\');" << '\n';
    stream << "    if (idx != std::string::npos)" << '\n';
    stream << "    {" << '\n';
    stream << "        type = type.substr(idx + 1);" << '\n';
    stream << "    }" << '\n';
    stream << "    auto it = typeMap.find(type);" << '\n';
    stream << "    if (it != typeMap.end())" << '\n';
    stream << "    {" << '\n';
    stream << "        obj.Value = it->second(objNode);" << '\n';
    stream << "    }" << '\n';
}

static void GenerateWriteBasic(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    stream << "    if (obj.Value)" << '\n';
    stream << "    {" << '\n';
    stream << "        obj.Value->ToAnyXml(doc, objNode);" << '\n';
    stream << "    }" << '\n';
}

void GenerateParser(
    std::ostream& stream,
    const Options& options,
    const Definition& definition,
    bool isStatic,
    const std::string& typePrefix,
    const std::string& typeSuffix)
{
    auto typeName = "SoapLibAnyType";
    auto parameterType = typePrefix + typeName + typeSuffix;

    stream << "static void addNamespace(" << '\n';
    stream << "    soaplib::xml::Node& node," << '\n';
    stream << "    const char* prefix," << '\n';
    stream << "    const char* href)" << '\n';
    stream << "{" << '\n';
    stream << "    auto np = node.GetXmlNode();" << '\n';
    stream << "    xmlSetNs(np, xmlNewNs(np, BAD_CAST href, BAD_CAST prefix));" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    if (isStatic)
    {
        stream << "static ";
    }

    stream << "void " << typeName << "FromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& objNode," << '\n';
    stream << "    " << parameterType << "& obj)" << '\n';
    stream << "{" << '\n';

    GenerateParseBasic(stream, options, definition);

    stream << "}" << '\n';
    stream << '\n';

    if (isStatic)
    {
        stream << "static ";
    }

    stream << parameterType << " " << typeName << "FromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& objNode)" << '\n';
    stream << "{" << '\n';
    stream << "    " << parameterType << " obj;" << '\n';
    stream << "    " << typeName << "FromXml(objNode, obj);" << '\n';
    stream << "    " << "return obj;" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    if (isStatic)
    {
        stream << "static ";
    }

    stream << "std::unique_ptr<soaplib::SoapBaseType> " << typeName << "PtrFromXml(" << '\n';
    stream << "    " << "const soaplib::xml::Node& node)" << '\n';
    stream << "{" << '\n';
    stream << "    return {}; // TODO" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    stream << "static void _" << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << parameterType << "& obj," << '\n';
    stream << "    " << "soaplib::xml::Document& doc," << '\n';
    stream << "    " << "soaplib::xml::Node& objNode)" << '\n';
    stream << "{" << '\n';

    GenerateWriteBasic(stream, options, definition);

    stream << "}" << '\n';
    stream << '\n';

    if (isStatic)
    {
        stream << "static ";
    }

    stream << "void " << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << parameterType << "& obj," << '\n';
    stream << "    " << "soaplib::xml::Document& doc," << '\n';
    stream << "    " << "soaplib::xml::Node& parentNode, " << '\n';
    stream << "    " << "bool createNode)" << '\n';
    stream << "{" << '\n';
    stream << "    if (createNode)" << '\n';
    stream << "    {" << '\n';
    stream << "        "
           << "auto objNode = soaplib::addChild(doc, parentNode, "
           << "\""
           << typeName
           << "\", \""
           << ""
           << "\", \""
           << ""
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

static void GenerateTypeMap(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    stream << "static std::map<std::string, std::unique_ptr<soaplib::SoapBaseType>(*)(const soaplib::xml::Node&)> typeMap =" << '\n';
    stream << "{" << '\n';

    for (const auto& type : definition.types)
    {
        if (!type)
        {
            continue;
        }

        // if (IsNativeType(type->name))
        // {
        //     continue;
        // }

        auto typeName = ResolveType(type->name, true);

        stream << "    { \"" << typeName << "\", " << typeName << "PtrFromXml" << " }," << '\n';
    }

    stream << "};" << '\n';
}

void GenerateAnyTypeImplementation(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    auto typeName = "SoapLibAnyType";

    stream << "// " << typeName << '\n';
    if (options.writeTimestamp)
    {
        stream << "// " << now() << '\n';
    }
    stream << '\n';
    stream << "#include <string>" << '\n';
    stream << "#include <map>" << '\n';
    stream << "#include \"" << typeName << ".hpp\"" << '\n';
    stream << '\n';
    stream << "#include <soaplib/basicTypes.hpp>" << '\n';
    stream << "#include <soaplib/parseHelper.hpp>" << '\n';
    stream << '\n';

    for (const auto& type : definition.types)
    {
        if (type && !IsNativeType(type->name))
        {
            stream << "#include \"" << ResolveType(type->name, true) << ".hpp\"" << '\n';
        }
    }

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    stream << "using namespace ::soaplib;" << '\n';

    stream << '\n';

    GenerateTypeMap(stream, options, definition);

    stream << '\n';

    GenerateParser(stream, options, definition, false, {}, {});

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}

} // namespace type
} // namespace cppgen
