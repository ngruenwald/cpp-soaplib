#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <soaplib/xml/xml.hpp>

namespace soaplib {

/// Tests if the given XML node is valid.
/// @param[in] node The XML node to test
/// @returns True if valid, otherwise false
bool isValidNode(
    const xml::Node& node);

/// Tests if the given node has a "nil" attribute set.
/// @param[in] node The XML node to test
/// @returns True if the "nil" attribute is present, otherwise false
bool isNil(
    const xml::Node& node);

/// Retrieves the date of a child element, or throws if not possible.
/// @tparam T The type of the element data.
/// @param[in] parentNode The XML parent element
/// @param[in] childName The name of the child element
/// @param[in] fromXml The XML parser used to decode the element data
/// @returns The value of the child element
/// @throws xml::Exception
/// @throws std::runtime_error, std::invalid_argument, std::out_of_range, ...
template<typename T>
T getMandatory(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    return fromXml(parentNode.GetChild(childName.c_str()));
}

/// Retrieves the date of a child element as pointer, if available.
/// @tparam T The type of the element data.
/// @param[in] parentNode The XML parent element
/// @param[in] childName The name of the child element
/// @param[in] fromXml The XML parser used to decode the element data
/// @returns An unique pointer containing the data, if available.
template<typename T>
std::unique_ptr<T> getPointer(
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

        return std::make_unique<T>(fromXml(node));
    }
    catch (const std::exception&)
    {
        return {};
    }
}

/// Retrieves the data of a child element, if available.
/// @tparam T The type of the element data.
/// @param[in] parentNode The XML parent element
/// @param[in] childName The name of the child element
/// @param[in] fromXml The XML parser used to decode the element data
/// @returns An optional containing the data, if available.
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

/// Retrieves the data of multiple child elements.
/// @tparam T The type of the element data.
/// @param[in] parentNode The XML parent element
/// @param[in] childName The name of the child elements
/// @param[in] fromXml The XML parser used to decode the element data
/// @returns A vector containing the data of the child elements.
/// @throws std::runtime_error, std::invalid_argument, std::out_of_range, ...
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

/// Retrieves the data of multiple child elements as pointers.
/// @tparam T The type of the element data.
/// @param[in] parentNode The XML parent element
/// @param[in] childName The name of the child elements
/// @param[in] fromXml The XML parser used to decode the element data
/// @returns A vector containing the data of the child elements.
/// @throws std::runtime_error, std::invalid_argument, std::out_of_range, ...
template<typename T>
std::vector<std::unique_ptr<T>> getMultiplePtrs(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    std::vector<std::unique_ptr<T>> result;

    const auto& childNodes = parentNode.GetChildren(childName.c_str());

    for (const auto& childNode : childNodes)
    {
        result.push_back(std::make_unique<T>(fromXml(childNode)));
    }

    return result;
}

/// Create unique pointer from value.
/// @tparam T The type of the value
/// @param[in] value
/// @returns A pointer holding the value.
/// @throws std::bad_alloc, ...
template<typename T>
std::unique_ptr<T> set(
    const T& value)
{
    return std::make_unique<T>(value);
}

/// Adds a namespace to a XML node.
/// @param[in] doc XML document
/// @param[in] node The XML node
/// @param[in] href Namespace URL
/// @param[in] prefix Namespace prefix
void addNamespace(
    xml::Document& doc,
    xml::Node& node,
    const std::string& href,
    const std::string& prefix);

/// Adds a child to a parent node.
/// @param[in] doc XML document
/// @param[in] parentNode The XML parent node
/// @param[in] name The name of the child element
/// @param[in] href Namespace URL
/// @param[in] prefix Namespace prefix
/// @returns The newly created child element
/// @throws xml::Exception (?)
xml::Node addChild(
    xml::Document& doc,
    xml::Node& parentNode,
    const std::string& name,
    const std::string& href,
    const std::string& prefix);

/// Helper method to set the namespace and type attribute on an "any" node
/// @param[in] doc XML document
/// @param[in] anyNode The XML node
/// @param[in] type The type name
/// @param[in] href Namespace URL of the type
/// @param[in] prefix Namespace prefix
void setAnyTypeAttribute(
    xml::Document& doc,
    xml::Node& anyNode,
    const std::string& type,
    const std::string& href,
    const std::string& prefix);

} // namespace soaplib
