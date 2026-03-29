// ArrayOftCountryInfo
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "tCountryInfo.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ArrayOftCountryInfo
    : soaplib::SoapBaseType
{
    std::vector<::country::tCountryInfo> TCountryInfo;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryInfo& obj);

ArrayOftCountryInfo ArrayOftCountryInfoFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftCountryInfoToXml(
    const ArrayOftCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
