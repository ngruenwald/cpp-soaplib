// {{ type.name }}
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}

#include <string>
#include <map>
#include "{{ type.name }}.hpp"

#include <soaplib/basicTypes.hpp>
#include <soaplib/parseHelper.hpp>

{% for t in definition.types %}
{% if not t.isNativeType %}
#include "{{ t.resolved_name }}.hpp"
{% endif %}
{% endfor %}

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

using namespace ::soaplib;

static std::map<std::string, std::unique_ptr<soaplib::SoapBaseType>(*)(const soaplib::xml::Node&)> typeMap =
{
{% for t in definition.types %}
    { "{{ t.resolved_name }}", {{ t.resolved_name }}PtrFromXml },
{% endfor %}
};

static void addNamespace(
    soaplib::xml::Node& node,
    const char* prefix,
    const char* href)
{
    auto np = node.GetXmlNode();
    xmlSetNs(np, xmlNewNs(np, BAD_CAST href, BAD_CAST prefix));
}

void {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode,
    {{ type.name }}& obj)
{
    auto type = objNode.GetStringProp("type");
    auto idx = type.find(':');
    if (idx != std::string::npos)
    {
        type = type.substr(idx + 1);
    }
    auto it = typeMap.find(type);
    if (it != typeMap.end())
    {
        obj.Value = it->second(objNode);
    }
}

{{ type.name }} {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode)
{
    {{ type.name }} obj;
    {{ type.name }}FromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> {{ type.name }}PtrFromXml(
    const soaplib::xml::Node& node)
{
    return {}; // TODO
}

static void _{{ type.name }}ToXml(
    const {{ type.name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    if (obj.Value)
    {
        obj.Value->ToAnyXml(doc, objNode);
    }
}

void {{ type.name }}ToXml(
    const {{ type.name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "{{ type.name }}", "", "");
        _{{ type.name }}ToXml(obj, doc, objNode);
    }
    else
    {
        _{{ type.name }}ToXml(obj, doc, parentNode);
    }
}

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
