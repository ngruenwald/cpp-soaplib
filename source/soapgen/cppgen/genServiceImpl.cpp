#include "genServiceImpl.hpp"

#include "typeMap.hpp"
#include "utils.hpp"

#include "genServiceHelper.hpp"

namespace cppgen {
namespace service {

void GenerateImplementation(
    std::ostream& stream,
    const Service& service,
    const Options& options,
    const Definition& definition)
{
    stream << "// " << service.name.name << '\n';
    stream << "// " << now() << '\n';
    stream << '\n';
    stream << "#include \"" << service.name.name << ".hpp\"" << '\n';
    stream << '\n';
    stream << "#include <soaplib/parseHelper.hpp>" << '\n';
    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    stream << service.name.name << "::" << service.name.name << "(" << '\n';
    stream << "    const std::string& serviceAddress)" << '\n';
    stream << "    : soaplib::SoapService(serviceAddress)" << '\n';
    stream << "{" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    stream << service.name.name << "::~" << service.name.name << "()" << '\n';
    stream << "{" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    std::string nspref = "::";
    for (const auto& ns : options.namespaces)
    {
        nspref += ns + "::";
    }

    for (const auto& port : service.ports)
    {
        const auto binding = getBinding(port.binding, definition);
        if (!binding)
        {
            continue;
        }

        const auto portType = getPortType(binding->type, definition);
        if (!portType)
        {
            continue;
        }

        for (const auto operation : portType->operations)
        {
            auto inputType = getMessagePartNames(operation.input.message, definition)[0];
            auto outputType = getMessagePartNames(operation.output.message, definition)[0];

            stream
                << nspref
                << ResolveType(outputType)
                << " "
                << service.name.name
                << "::"
                << operation.name.name
                << "("
                << '\n';

            stream
                << "    const "
                << nspref
                << ResolveType(inputType)
                << "& input)"
                << '\n';

            stream << "{" << '\n';

            stream << "    xml::Document request;" << '\n';
            stream << "    auto body = CreateEnvelope(request, \"" << ResolveType(inputType) << "\");" << '\n';
            stream << "    AddChild(request, body, \"" << ResolveType(inputType) << "\", \"t\");" << '\n';
            stream << '\n';
            stream << "    auto response = Call(request);" << '\n';
            stream << "    auto envelope = response->GetRootNode();" << '\n';
            stream << "    auto operation = envelope.GetChild(\"Body\").GetChild(\"" << ResolveType(outputType) << "\");" << '\n';
            stream << '\n';
            stream << "    return " << nspref << ResolveType(outputType) << "FromXml(operation);" << '\n';
            stream << "}" << '\n';

            stream << '\n';
        }
    }

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}

} // namespace service
} // namespace cppgen
