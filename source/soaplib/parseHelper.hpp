#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <libxmlwrp.hpp>

namespace soaplib {

inline bool isValidNode(
    const xml::Node& node)
{
    return node.GetXmlNode() != nullptr;
}

inline bool isNil(
    const xml::Node& node)
{
    try
    {
        return node.GetBoolProp("s:nil"); // TODO: use correct namespace
    }
    catch(const xml::Exception&)
    {
        return false;
    }
}

template<typename T>
T getMandatory(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    return fromXml(parentNode.GetChild(childName.c_str()));
}

template<typename T>
std::shared_ptr<T> getPointer(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    try
    {
        auto node = parentNode.GetChild(childName.c_str());

        if (!isValidNode(node))
        {
            return {};
        }

        if (isNil(node))
        {
            return {};
        }

        return std::make_shared<T>(fromXml(node));
    }
    catch (const std::exception&)
    {
        return {};
    }
}

template<typename T>
std::optional<T> getOptional(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    try
    {
        auto node = parentNode.GetChild(childName.c_str());

        if (!isValidNode(node))
        {
            return {};
        }

        if (isNil(node))
        {
            return {};
        }

        return std::optional<T>(fromXml(node));
    }
    catch (const std::exception&)
    {
    }
}

template<typename T>
std::vector<T> getMultiple(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    std::vector<T> result;

    const auto& childNodes = parentNode.GetChildren(childName.c_str());

    for (const auto& childNode : childNodes)
    {
        result.push_back(fromXml(childNode));
    }

    return result;
}

template<typename T>
std::vector<std::shared_ptr<T>> getMultiplePtrs(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    std::vector<std::shared_ptr<T>> result;

    const auto& childNodes = parentNode.GetChildren(childName.c_str());

    for (const auto& childNode : childNodes)
    {
        result.push_back(std::make_shared<T>(fromXml(childNode)));
    }

    return result;
}

template<typename T>
std::shared_ptr<T> set(
    const T& value)
{
    return std::make_shared<T>(value);
}

inline void addNamespace(
	xml::Document& doc,
	xml::Node& node,
	const std::string& href,
	const std::string& prefix)
{
	auto np = node.GetXmlNode();

    if (!prefix.empty())
    {
        auto ns = xmlSearchNs(doc.GetDoc(), np, BAD_CAST prefix.c_str());
		if (ns)
		{
			xmlSetNs(np, ns);
            return;
		}
    }

	if (!href.empty())
	{
		auto ns = xmlSearchNsByHref(doc.GetDoc(), np, BAD_CAST href.c_str());
        if (ns)
        {
            xmlSetNs(np, ns);
            return;
        }
	}

    if (href.empty() || prefix.empty())
    {
        return;
    }

	xmlSetNs(np, xmlNewNs(np, BAD_CAST href.c_str(), BAD_CAST prefix.c_str()));
}

inline xml::Node addChild(
	xml::Document& doc,
	xml::Node& parentNode,
	const std::string& name,
    const std::string& href,
    const std::string& prefix)
{
	auto node = parentNode.AddChild(name.c_str());
    if (!href.empty() || !prefix.empty())
    {
	    addNamespace(doc, node, href, prefix);
    }
	return node;
}

} // namespace soaplib
