#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <soaplib/xml/xml.hpp>

namespace soaplib {

bool isValidNode(
    const xml::Node& node);

bool isNil(
    const xml::Node& node);

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

    return {};
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

void addNamespace(
	xml::Document& doc,
	xml::Node& node,
	const std::string& href,
	const std::string& prefix);

xml::Node addChild(
	xml::Document& doc,
	xml::Node& parentNode,
	const std::string& name,
    const std::string& href,
    const std::string& prefix);

} // namespace soaplib
