{% if kind == "struct" %}
{% for innerType in struct.innerTypes %}
{{ fmacro("TypeImpl_Serializer.tpl", innerType) }}
{% endfor %}
{% endif %}

void {{ name }}FromXml(
    const soaplib::xml::Node& objNode,
    {{ name }}& obj)
{
{% if length(default(base.name, "")) > 0 and base.name != "soaplib::SoapBaseType" %}
    {{ base.resolved_name }}FromXml(objNode, obj);
{% endif %}
{% if kind == "enum" %}
    auto s = StringFromXml(objNode);
{% for enumeration in enum.enumerations %}
    {% if not loop.is_first %}else {% endif %}if (s == "{{ enumeration.wsdl_text }}")
    {
        obj.Value = {{ name }}::{{ enumeration.text }};
    }
{% endfor %}
{% else if kind == "struct" %}
{% for parameter in struct.parameters %}
    obj.{{ parameter.name }} = {% if false -%}
    {%- else if parameter.kind == "mandatory" %}soaplib::getMandatory{#- -#}
    {%- else if parameter.kind == "pointer" %}soaplib::getPointer{#- -#}
    {%- else if parameter.kind == "optional" %}{% if parameter.isPointerType %}soaplib::getPointer{% else %}soaplib::getOptional{% endif -%}
    {%- else if parameter.kind == "multiple" %}{% if parameter.isPointerType %}soaplib::getMultiplePtrs{% else %}soaplib::getMultiple{% endif -%}
    {%- else %}soaplib::getMandatory{#- -#}
    {%- endif %}<{{ parameter.prefix}}{% if parameter.isInnerType %}{{ name }}::{% endif %}{{ parameter.full_resolved_type }}{{ parameter.suffix }}>(objNode, "{{ parameter.wsdl_name }}", {{ parameter.type }}FromXml);
{% endfor %}
{% endif %}
}

{{ name }} {{ name }}FromXml(
    const soaplib::xml::Node& objNode)
{
    {{ name }} obj;
    {{ name }}FromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> {{ name }}PtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<{{ name }}>();
    {{ name }}FromXml(objNode, *obj.get());
    return obj;
}

static void _{{ name }}ToXml(
    const {{ name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
{% if length(default(base.name, "")) > 0 and base.name != "soaplib::SoapBaseType" %}
    _{{ base.resolved_name }}ToXml(obj, doc, objNode);
{% endif %}
{% if kind == "enum" %}
    soaplib::String s;
    switch (obj.Value)
    {
{% for enumeration in enum.enumerations %}
        case {{ name }}::{{ enumeration.text }}:
        {
            s = "{{ enumeration.wsdl_text }}";
            break;
        }
{% endfor %}
        default: break;
    }
    StringToXml(objNode, s);
{% else if kind == "struct" %}
{% for parameter in struct.parameters %}
{% if parameter.kind == "mandatory" %}
    {
        auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
        {{ parameter.type }}ToXml(pn, obj.{{ parameter.name }});
{% else %}
        {{ parameter.type }}ToXml(obj.{{ parameter.name }}, doc, pn, false);
{% endif %}
    }
{% else if parameter.kind == "optional" %}
{% if parameter.isPointerType %}
    if (obj.{{ parameter.name }})
    {
        auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
        {{ parameter.type }}ToXml(pn, *obj.{{ parameter.name }});
{% else %}
        {{ parameter.type }}ToXml(*obj.{{ parameter.name }}, doc, pn, false);
{% endif %}
    }
{% else %}
    if (obj.{{ parameter.name }}.has_value())
    {
        auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
        {{ parameter.type }}ToXml(pn, obj.{{ parameter.name }}.value());
{% else %}
        {{ parameter.type }}ToXml(obj.{{ parameter.name }}.value(), doc, pn, false);
{% endif %}
    }
{% endif %}
{% else if parameter.kind == "pointer" %}
    if (obj.{{ parameter.name }})
    {
        auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
        {{ parameter.type }}ToXml(pn, *obj.{{ parameter.name }});
{% else %}
        {{ parameter.type }}ToXml(*obj.{{ parameter.name }}, doc, pn, false);
{% endif %}
    }
{% else if parameter.kind == "multiple" %}
{% if parameter.isPointerType %}
    for (const auto& entry : obj.{{ parameter.name }})
    {
        if (entry)
        {
            auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
            {{ parameter.type }}ToXml(pn, *entry);
{% else %}
            {{ parameter.type }}ToXml(*entry, doc, pn, false);
{% endif %}
        }
    }
{% else %}
    for (const auto& entry : obj.{{ parameter.name }})
    {
        auto pn = soaplib::addChild(doc, objNode, "{{ parameter.wsdl_name }}", "{{ parameter.xmlns.href }}", "{{ parameter.xmlns.prefix }}");
{% if parameter.isNativeType %}
        {{ parameter.type }}ToXml(pn, entry);
{% else %}
        {{ parameter.type }}ToXml(entry, doc, pn, false);
{% endif %}
    }
{% endif %}
{% endif %}
{% endfor %}
{% endif %}
}

void {{ name }}ToXml(
    const {{ name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "{{ wsdl_name }}", "{{ xmlns.href }}", "{{ xmlns.prefix }}");
        _{{ name }}ToXml(obj, doc, objNode);
    }
    else
    {
        _{{ name }}ToXml(obj, doc, parentNode);
    }
}

void {{ name }}::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "{{ name }}", "{{ xmlns.href }}", "{{ xmlns.prefix }}");
    _{{ name }}ToXml(*this, doc, node);
}
