// ArrayOftCountryCodeAndNameGroupedByContinent
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "tCountryCodeAndNameGroupedByContinent.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ArrayOftCountryCodeAndNameGroupedByContinent
    : soaplib::SoapBaseType
{
    std::vector<::country::tCountryCodeAndNameGroupedByContinent> TCountryCodeAndNameGroupedByContinent;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryCodeAndNameGroupedByContinent& obj);

ArrayOftCountryCodeAndNameGroupedByContinent ArrayOftCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryCodeAndNameGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftCountryCodeAndNameGroupedByContinentToXml(
    const ArrayOftCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
