// ListOfCountryNamesByCode
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCountryNamesByCode
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByCode& obj);

ListOfCountryNamesByCode ListOfCountryNamesByCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesByCodeToXml(
    const ListOfCountryNamesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
