// {{ service.name }}Server
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}

#include "{{ service.name }}Server.hpp"

#include <soaplib/soapException.hpp>
#include <soaplib/parseHelper.hpp>

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

using namespace ::soaplib;

{{ service.name }}Server::{{ service.name }}Server()
{
}

{{ service.name }}Server::~{{ service.name }}Server()
{
}

std::unique_ptr<soaplib::xml::Document> {{ service.name }}Server::HandleRequest(
    const soaplib::xml::Document& request)
{
    auto envelope = request.GetRootNode();
    auto body = envelope.GetChild("Body");
    auto operationNode = body.GetChildren()[0];
    std::string operationName = operationNode.GetName();

{% for op in service.operations %}
    {% if not loop.is_first %}else {% endif %}if (operationName == "{{ op.input_type }}")
    {
        // Parse input
        auto input = ::{{ concat(options.namespaces, "::", true) }}{{ op.input_resolved_type }}FromXml(operationNode);
        
        // Call implementation
        {% if op.output_resolved_type != "void" -%}
        auto result = {{ op.name }}(input);
        {%- else -%}
        {{ op.name }}(input);
        {%- endif %}

        // Create response
        auto response = std::make_unique<soaplib::xml::Document>();
        auto responseBody = CreateEnvelope(*response, "{{ op.output.action }}");
        
        {% if op.output_resolved_type != "void" -%}
        {{ op.output_resolved_type }}ToXml(result, *response, responseBody, true);
        {%- endif %}
        
        return response;
    }
{% endfor %}

    throw soaplib::SoapException("Unknown operation: " + operationName);
}

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
