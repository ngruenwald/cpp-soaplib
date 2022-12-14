#include "genServiceHeader.hpp"

#include "typeMap.hpp"
#include "utils.hpp"

#include "genServiceHelper.hpp"

namespace cppgen {
namespace service {

void GenerateHeader(
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
    stream << "#pragma once" << '\n';
    stream << '\n';

    stream << "#include <soaplib/soapService.hpp>" << '\n';
    stream << '\n';

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

        for (const auto& operation : portType->operations)
        {
            const auto inputTypes = getMessagePartNames(operation.input.message, definition);
            for (const auto& inputType : inputTypes)
            {
                stream
                    << "#include \"types/"
                    << /*definition.name.name << "_" <<*/ ResolveType(inputType) << ".hpp"
                    << "\""
                    << '\n';
            }

            if (!operation.output.message.name.empty())
            {
                const auto outputTypes = getMessagePartNames(operation.output.message, definition);
                for (const auto& outputType : outputTypes)
                {
                    stream
                        << "#include \"types/"
                        << /*definition.name.name << "_" <<*/ ResolveType(outputType) << ".hpp"
                        << "\""
                        << '\n';
                }
            }
        }
    }

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "namespace " << ns << " {" << '\n';
    }

    stream << '\n';

    stream << "class " << serviceName << '\n';
    stream << "    : public soaplib::SoapService" << '\n';
    stream << "{" << '\n';
    stream << "public:" << '\n';
    stream << "    " << serviceName << "(" << '\n';
    stream << "        const std::string& serviceAddress);" << '\n';
    stream << '\n';
    stream << "    ~" << serviceName << "();" << '\n';
    stream << '\n';
    stream << "public:" << '\n';

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

        for (const auto& operation : portType->operations)
        {
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
                << "    "
                << _nspref
                << ResolveType(outputType)
                << " "
                << operation.name.name
                << "("
                << '\n';

            stream
                << "        const "
                << nspref
                << ResolveType(inputType)
                << "& input);"
                << '\n';

            stream << '\n';
        }
    }
    stream << "};" << '\n';

    stream << '\n';

    for (const auto& ns : options.namespaces)
    {
        stream << "} // namespace " << ns << '\n';
    }
}

} // namespace service
} // namespace cppgen
