// ListOfCountryNamesGroupedByContinentResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCountryCodeAndNameGroupedByContinent.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct ArrayOftCountryCodeAndNameGroupedByContinent;

struct ListOfCountryNamesGroupedByContinentResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCountryCodeAndNameGroupedByContinent ListOfCountryNamesGroupedByContinentResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesGroupedByContinentResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesGroupedByContinentResponse& obj);

ListOfCountryNamesGroupedByContinentResponse ListOfCountryNamesGroupedByContinentResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesGroupedByContinentResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesGroupedByContinentResponseToXml(
    const ListOfCountryNamesGroupedByContinentResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
