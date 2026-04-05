// CountryISOCode
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryISOCode
    : soaplib::SoapBaseType
{
    soaplib::String SCountryName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryISOCodeFromXml(
    const soaplib::xml::Node& objNode,
    CountryISOCode& obj);

CountryISOCode CountryISOCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryISOCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryISOCodeToXml(
    const CountryISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
