{% if default(base.name, "soaplib::SoapBaseType") != "soaplib::SoapBaseType" and not base.isNativeType %}
#include "{{ base.name }}.hpp"
{% endif %}
{% if kind == "struct" %}
{% for innerType in struct.innerTypes %}
{{ fmacro("TypeHeader_Includes.tpl", innerType) }}
{% endfor %}
{% for parameter in struct.parameters %}
{% if not parameter.isNativeType and not parameter.isInnerType %}
#include "{{ parameter.type }}.hpp"
{% endif %}
{% endfor %}
{% endif %}
