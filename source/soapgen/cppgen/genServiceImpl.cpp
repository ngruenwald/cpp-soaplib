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
    const std::string serviceName = safe_name(service.name.name);

    stream << "// " << serviceName << '\n';
    if (options.writeTimestamp)
    {
        stream << "// " << now() << '\n';
    }
    stream << '\n';
    stream << "#include \"" << serviceName << ".hpp\"" << '\n';
    stream << '\n';
    stream << "#include <soaplib/parseHelper.hpp>" << '\n';
    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    stream << "using namespace ::soaplib;" << '\n';

    stream << '\n';

    std::string contract = "";

    stream << serviceName << "::" << serviceName << "(" << '\n';
    stream << "    const std::string& serviceAddress)" << '\n';
    stream << "    : soaplib::SoapService(serviceAddress, \"" << contract << "\")" << '\n';
    stream << "{" << '\n';
    stream << "}" << '\n';
    stream << '\n';

    stream << serviceName << "::~" << serviceName << "()" << '\n';
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
        if (!isPortAllowed(options, port))
        {
            continue;
        }

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
            const auto& soapAction = operation.input.action.name;

            if (operation.input.message.name.empty())
            {
                continue;
            }

            auto inputType = getMessagePartNames(operation.input.message, definition)[0];

            Name outputType;
            if (!operation.output.message.name.empty())
            {
                outputType = getMessagePartNames(operation.output.message, definition)[0];
            }

            auto _nspref = nspref;

            if (IsNativeType(outputType))
            {
                _nspref = "";
            }

            stream
                << _nspref
                << ResolveType(outputType)
                << " "
                << serviceName
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

            stream << "    soaplib::xml::Document request;" << '\n';
            stream << "    auto body = CreateEnvelope(request, \"" << soapAction << "\");" << '\n';
            //stream << "    AddChild(request, body, \"" << ResolveType(inputType) << "\", \"t\");" << '\n';
            stream << "    " << ResolveType(inputType) << "ToXml(input, request, body, true);" << '\n';
            stream << '\n';
            stream << "    auto response = Call(request);" << '\n';
            stream << "    auto envelope = response->GetRootNode();" << '\n';
            stream << "    auto operation = envelope.GetChild(\"Body\").GetChild(\"" << ResolveType(outputType) << "\");" << '\n';
            stream << '\n';
            if (ResolveType(outputType) != "void")    // TODO: same as above?
            {
                stream << "    return " << _nspref << ResolveType(outputType) << "FromXml(operation);" << '\n';
            }
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
