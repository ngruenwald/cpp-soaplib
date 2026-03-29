// ArrayOftCountryCodeAndName
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "tCountryCodeAndName.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ArrayOftCountryCodeAndName
    : soaplib::SoapBaseType
{
    std::vector<::country::tCountryCodeAndName> TCountryCodeAndName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryCodeAndName& obj);

ArrayOftCountryCodeAndName ArrayOftCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryCodeAndNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftCountryCodeAndNameToXml(
    const ArrayOftCountryCodeAndName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
