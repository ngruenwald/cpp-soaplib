// {{ type.name }}
{% if options.writeTimestamp %}
// {{ meta.version }}
// {{ meta.timestamp }}
{% endif %}

#include "{{ type.name }}.hpp"

#include <soaplib/parseHelper.hpp>

{{ fmacro("TypeImpl_Includes.tpl", type) }}
{% for namespace in type.namespaces -%}
namespace {{ namespace }} {
{% endfor %}
{{- fmacro("TypeImpl_Serializer.tpl", type) }}
{% for namespace in type.namespaces -%}
} // namespace {{ namespace }}
{% endfor %}
