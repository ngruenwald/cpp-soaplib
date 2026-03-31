// {{ service.name }}Server
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}

#pragma once

#include <soaplib/SoapServer.hpp>

{% for op in service.operations %}
#include "types/{{ op.input_resolved_type }}.hpp"
{% if op.output_resolved_type != "void" and not op.output_is_native %}
#include "types/{{ op.output_resolved_type }}.hpp"
{% endif %}
{% endfor %}

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

/// The "{{ service.name }}" SOAP service server stub
class {{ service.name }}Server
    : public soaplib::SoapServer
{
public:
    /// Constructs a new instance of the "{{ service.name }}" service server stub.
    {{ service.name }}Server();

    /// Destructs the service instance.
    virtual ~{{ service.name }}Server();

public:
    /// Handles a SOAP request.
    /// @param[in] request The request XML document
    /// @returns The response XML document
    std::unique_ptr<soaplib::xml::Document> HandleRequest(
        const soaplib::xml::Document& request) override;

protected:
{% for op in service.operations %}
    /// Implementation of the "{{ op.name }}" operation.
  {% if not op.output_is_native %}
    virtual ::{{ concat(options.namespaces, "::", true) }}{% endif %}{{ op.output_resolved_type }} {{ op.name }}(
        const ::{{ concat(options.namespaces, "::", true) }}{{ op.input_resolved_type }}& input) = 0;

{% endfor %}
};

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
