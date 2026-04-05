// {{ type.name }}
{% if options.writeTimestamp %}
// {{ meta.version }}
// {{ meta.timestamp }}
{% endif %}
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

{%- if length(default(type.base.name, "")) > 0 and type.base.name != "soaplib::SoapBaseType" and not type.base.isNativeType %}
#include "{{ type.base.name }}.hpp"
{% endif -%}
{{ fmacro("TypeHeader_Includes.tpl", type) }}
namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

{% for namespace in type.namespaces %}
namespace {{ namespace }} {
{% endfor %}
{{ fmacro("TypeHeader_ForwardDeclarations.tpl", type) }}
{{ fmacro("TypeHeader_TypeDeclaration.tpl", type) }}
void {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode,
    {{ type.name }}& obj);

{{ type.name }} {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> {{ type.name }}PtrFromXml(
    const soaplib::xml::Node& objNode);

void {{ type.name }}ToXml(
    const {{ type.name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

{%- for namespace in type.namespaces %}
} // namespace {{ namespace }}
{% endfor -%}
