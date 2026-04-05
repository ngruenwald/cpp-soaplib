// ListOfCountryNamesByName
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCountryNamesByName
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByName& obj);

ListOfCountryNamesByName ListOfCountryNamesByNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesByNameToXml(
    const ListOfCountryNamesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
