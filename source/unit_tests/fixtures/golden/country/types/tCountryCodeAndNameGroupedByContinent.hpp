// tCountryCodeAndNameGroupedByContinent
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "tContinent.hpp"
#include "ArrayOftCountryCodeAndName.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct tContinent;
struct ArrayOftCountryCodeAndName;

struct tCountryCodeAndNameGroupedByContinent
    : soaplib::SoapBaseType
{
    ::country::tContinent Continent;
    ::country::ArrayOftCountryCodeAndName CountryCodeAndNames;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    tCountryCodeAndNameGroupedByContinent& obj);

tCountryCodeAndNameGroupedByContinent tCountryCodeAndNameGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tCountryCodeAndNameGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode);

void tCountryCodeAndNameGroupedByContinentToXml(
    const tCountryCodeAndNameGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
