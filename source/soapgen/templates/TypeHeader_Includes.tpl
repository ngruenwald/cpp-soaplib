{% if default(base.name, "soaplib::SoapBaseType") != "soaplib::SoapBaseType" %}
#include "{{ base.name }}.hpp"
{% endif %}
{% if kind == "struct" %}
{% for innerType in struct.innerTypes %}
{{ fmacro("TypeHeader_Includes.tpl", innerType) }}
{% endfor %}
{% for parameter in struct.parameters %}
{% if not parameter.isNativeType and not parameter.isPointerType and not parameter.isInnerType and not parameter.needsForwardDeclaration %}
#include "{{ parameter.type }}.hpp"
{% endif %}
{% endfor %}
{% endif %}
