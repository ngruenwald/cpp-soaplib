// FullCountryInfo
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct FullCountryInfo
    : soaplib::SoapBaseType
{
    soaplib::String SCountryISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void FullCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfo& obj);

FullCountryInfo FullCountryInfoFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode);

void FullCountryInfoToXml(
    const FullCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
