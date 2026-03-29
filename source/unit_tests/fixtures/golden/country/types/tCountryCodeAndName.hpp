// tCountryCodeAndName
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct tCountryCodeAndName
    : soaplib::SoapBaseType
{
    soaplib::String SISOCode;
    soaplib::String SName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode,
    tCountryCodeAndName& obj);

tCountryCodeAndName tCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tCountryCodeAndNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void tCountryCodeAndNameToXml(
    const tCountryCodeAndName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
