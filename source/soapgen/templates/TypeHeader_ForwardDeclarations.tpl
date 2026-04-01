{% if kind == "struct" %}
{% for innerType in struct.innerTypes %}
{{ fmacro("TypeHeader_ForwardDeclarations.tpl", innerType) }}
{% endfor %}
{% for parameter in struct.parameters %}
{% if not parameter.isNativeType and not parameter.isInnerType and (parameter.isPointerType or parameter.needsForwardDeclaration) %}
struct {{ parameter.type }};
{% endif %}
{% endfor %}
{% endif %}
