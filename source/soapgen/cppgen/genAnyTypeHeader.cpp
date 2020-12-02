#include "genAnyTypeHeader.hpp"

#include "typeMap.hpp"
#include "utils.hpp"

#include <iostream>

namespace cppgen {
namespace type {

void GenerateAnyTypeHeader(
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
    stream << "#pragma once" << '\n';
    stream << '\n';
    stream << "#include <memory>" << '\n';
    stream << "#include <soaplib/basicTypes.hpp>" << '\n';
    stream << '\n';

    stream << "namespace xml { class Document; }" << '\n';
    stream << "namespace xml { class Node; }" << '\n';
    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    stream << "struct " << typeName << '\n';
    stream << "{" << '\n';
    stream << "    std::shared_ptr<soaplib::SoapBaseType> Value;" << '\n';
    stream << "};" << '\n';

    stream << '\n';

    stream << "void " << typeName << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode," << '\n';
    stream << "    " << typeName << "& obj);" << '\n';

    stream << '\n';

    stream << typeName << " " << typeName << "FromXml(" << '\n';
    stream << "    " << "const xml::Node& objNode);" << '\n';

    stream << '\n';

    stream << "void " << typeName << "ToXml(" << '\n';
    stream << "    " << "const " << typeName << "& obj," << '\n';
    stream << "    " << "xml::Document& doc," << '\n';
    stream << "    " << "xml::Node& parentNode, " << '\n';
    stream << "    " << "bool createNode);" << '\n';

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}


} // namespace type
} // namespace cppgen
