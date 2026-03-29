// ListOfCurrenciesByName
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCurrenciesByName
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCurrenciesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByName& obj);

ListOfCurrenciesByName ListOfCurrenciesByNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCurrenciesByNameToXml(
    const ListOfCurrenciesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
