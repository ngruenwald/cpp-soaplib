// ListOfContinentsByName
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfContinentsByName
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfContinentsByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByName& obj);

ListOfContinentsByName ListOfContinentsByNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfContinentsByNameToXml(
    const ListOfContinentsByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
