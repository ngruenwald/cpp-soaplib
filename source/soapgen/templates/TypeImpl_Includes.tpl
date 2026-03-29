{% if kind == "struct" %}
{% for innerType in struct.innerTypes %}
{{ fmacro("TypeImpl_Includes.tpl", innerType) }}
{% endfor %}
{% for parameter in struct.parameters %}
{% if not parameter.isNativeType and not parameter.isInnerType %}
#include "{{ parameter.type }}.hpp"
{% endif %}
{% endfor %}
{% endif %}
