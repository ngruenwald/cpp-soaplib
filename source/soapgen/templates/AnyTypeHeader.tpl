// {{ type.name }}
{% if options.writeTimestamp %}// {{ meta.timestamp }}{% endif %}
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

{% for ns in options.namespaces %}
namespace {{ ns }} {
{% endfor %}

struct {{ type.name }}
{
    std::unique_ptr<soaplib::SoapBaseType> Value;

    template<typename T>
    T* ValueCast()
    {
        return static_cast<T*>(Value.get());
    }

    template<typename T>
    const T* ValueCast() const
    {
        return static_cast<const T*>(Value.get());
    }
};

void {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode,
    {{ type.name }}& obj);

{{ type.name }} {{ type.name }}FromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> {{ type.name }}PtrFromXml(
    const soaplib::xml::Node& node);

void {{ type.name }}ToXml(
    const {{ type.name }}& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

{% for ns in options.namespaces %}
} // namespace {{ ns }}
{% endfor %}
