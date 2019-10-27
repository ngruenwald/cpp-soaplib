#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <libxmlwrp.hpp>

namespace soaplib {

template<typename T>
std::shared_ptr<T> getMandatory(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    return std::make_shared<T>(fromXml(parentNode.GetChild(childName.c_str())));
}

template<typename T>
std::shared_ptr<T> getPointer(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    try
    {
        return std::make_shared<T>(fromXml(parentNode.GetChild(childName.c_str())));
    }
    catch (const xml::Exception&)
    {
        return {};
    }
}

template<typename T>
std::shared_ptr<T> getOptional(
    const xml::Node& parentNode,
    const std::string& childName,
    T (*fromXml)(const xml::Node&))
{
    try
    {
        return std::make_shared<T>(fromXml(parentNode.GetChild(childName.c_str())));
    }
    catch (const xml::Exception&)
    {
        return {};
    }
}

template<typename T>
std::vector<std::shared_ptr<T>> getMultiple(
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

} // namespace soaplib
