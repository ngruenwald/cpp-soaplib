// {{ service.name }}
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}

#pragma once

#include <soaplib/soapService.hpp>

{% for op in service.operations %}
#include "types/{{ op.input_resolved_type }}.hpp"
{% if op.output_resolved_type != "void" and not op.output_is_native %}
#include "types/{{ op.output_resolved_type }}.hpp"
{% endif %}
{% endfor %}

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

/// The "{{ service.name }}" SOAP service
class {{ service.name }}
    : public soaplib::SoapService
{
public:
    /// Constructs a new instance of the "{{ service.name }}" service.
    /// @param[in]  serviceAddress  URL of the SOAP service.
    {{ service.name }}(
        const std::string& serviceAddress);

    /// Destructs the service instance.
    ~{{ service.name }}();

public:
{% for op in service.operations %}
  {% if not op.output_is_native %}
    ::{{ concat(options.namespaces, "::", true) }}{% endif %}{{ op.output_resolved_type }} {{ op.name }}(
        const ::{{ concat(options.namespaces, "::", true) }}{{ op.input_resolved_type }}& input);

{% endfor %}
};

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
