// CountryName
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryName
    : soaplib::SoapBaseType
{
    soaplib::String SCountryISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryNameFromXml(
    const soaplib::xml::Node& objNode,
    CountryName& obj);

CountryName CountryNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryNameToXml(
    const CountryName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
