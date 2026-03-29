// {{ service.name }}
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}

#include "{{ service.name }}.hpp"

#include <soaplib/parseHelper.hpp>

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

using namespace ::soaplib;

{{ service.name }}::{{ service.name }}(
    const std::string& serviceAddress)
    : soaplib::SoapService(serviceAddress, "")
{
}

{{ service.name }}::~{{ service.name }}()
{
}

{% for op in service.operations %}
{% if not op.output_is_native %}::{{ concat(options.namespaces, "::", true) }}{% endif %}{{ op.output_resolved_type }} {{ service.name }}::{{ op.name }}(
    const ::{{ concat(options.namespaces, "::", true) }}{{ op.input_resolved_type }}& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "{{ op.input.action }}");
    {{ op.input_resolved_type }}ToXml(input, request, body, true);

    auto response = Call(request);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("{{ op.output_type }}");

{% if op.output_resolved_type != "void" %}
    return {% if not op.output_is_native %}::{{ concat(options.namespaces, "::", true) }}{% endif %}{{ op.output_resolved_type }}FromXml(operation);
{% endif %}
}

{% endfor %}

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
