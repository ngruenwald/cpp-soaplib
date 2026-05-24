// ListOfCountryNamesByNameResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct ArrayOftCountryCodeAndName;

struct ListOfCountryNamesByNameResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCountryCodeAndName ListOfCountryNamesByNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByNameResponse& obj);

ListOfCountryNamesByNameResponse ListOfCountryNamesByNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesByNameResponseToXml(
    const ListOfCountryNamesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
