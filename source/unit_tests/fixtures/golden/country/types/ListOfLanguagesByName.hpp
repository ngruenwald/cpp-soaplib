// ListOfLanguagesByName
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfLanguagesByName
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfLanguagesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByName& obj);

ListOfLanguagesByName ListOfLanguagesByNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfLanguagesByNameToXml(
    const ListOfLanguagesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
