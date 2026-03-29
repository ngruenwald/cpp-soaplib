// CapitalCity
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CapitalCity
    : soaplib::SoapBaseType
{
    soaplib::String SCountryISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CapitalCityFromXml(
    const soaplib::xml::Node& objNode,
    CapitalCity& obj);

CapitalCity CapitalCityFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CapitalCityPtrFromXml(
    const soaplib::xml::Node& objNode);

void CapitalCityToXml(
    const CapitalCity& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
