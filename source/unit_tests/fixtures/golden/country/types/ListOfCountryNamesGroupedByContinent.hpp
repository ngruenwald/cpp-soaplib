// ListOfCountryNamesGroupedByContinent
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCountryNamesGroupedByContinent
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesGroupedByContinent& obj);

ListOfCountryNamesGroupedByContinent ListOfCountryNamesGroupedByContinentFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesGroupedByContinentPtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesGroupedByContinentToXml(
    const ListOfCountryNamesGroupedByContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
