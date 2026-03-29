// SoapLibAnyType
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct SoapLibAnyType
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

void SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode,
    SoapLibAnyType& obj);

SoapLibAnyType SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> SoapLibAnyTypePtrFromXml(
    const soaplib::xml::Node& node);

void SoapLibAnyTypeToXml(
    const SoapLibAnyType& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
