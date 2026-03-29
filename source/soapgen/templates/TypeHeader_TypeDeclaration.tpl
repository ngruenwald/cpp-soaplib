{% if kind == "enum" %}
struct {{ name }}
    : {% if length(default(base.name, "")) > 0 %}{{ base.full_resolved_name }}{% else %}soaplib::SoapBaseType{% endif %}
{
    enum Values
    {
{% for enumeration in enum.enumerations %}
        {{ enumeration.text }},
{% endfor %}
    };

    Values Value;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};
{% else if kind == "basic" %}
struct {{ name }}
    : {% if length(default(base.name, "")) > 0 %}{{ base.full_resolved_name }}{% else %}soaplib::SoapBaseType{% endif %}
{
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};
{% else if kind == "struct" %}
struct {{ name }}
    : {% if length(default(base.name, "")) > 0 %}{{ base.full_resolved_name }}{% else %}soaplib::SoapBaseType{% endif %}

{
{% for innerType in struct.innerTypes %}
{{ indent(4, fmacro("TypeHeader_TypeDeclaration.tpl", innerType)) }}
{% endfor %}
{% for parameter in struct.parameters %}
  {% if parameter.kind == "mandatory" %}
    {% if not parameter.isNativeType and not parameter.isInnerType %}
    ::{{ concat(namespaces, "::", true) }}{{ parameter.full_resolved_type }} {{ parameter.name }};
    {% else %}
    {{ parameter.full_resolved_type }} {{ parameter.name }};
    {% endif %}
  {% else if parameter.kind == "pointer" %}
    std::unique_ptr<{% if not parameter.isNativeType and not parameter.isInnerType %}::{{ concat(namespaces, "::", true) }}{% endif %}{{ parameter.full_resolved_type }}> {{ parameter.name }};
  {% else if parameter.kind == "optional" %}
    std::{% if parameter.isPointerType %}unique_ptr{% else %}optional{% endif %}<{% if not parameter.isNativeType and not parameter.isInnerType %}::{{ concat(namespaces, "::", true) }}{% endif %}{{ parameter.full_resolved_type }}> {{ parameter.name }};
  {% else if parameter.kind == "multiple" %}
    std::vector<{% if parameter.isPointerType %}std::unique_ptr<{% endif %}{% if not parameter.isNativeType and not parameter.isInnerType %}::{{ concat(namespaces, "::", true) }}{% endif %}{{ parameter.full_resolved_type }}{% if parameter.isPointerType %}>{% endif %}> {{ parameter.name }};
  {% endif %}
{% endfor %}

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};
{% endif %}
