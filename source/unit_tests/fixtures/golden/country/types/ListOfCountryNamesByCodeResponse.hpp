// ListOfCountryNamesByCodeResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct ArrayOftCountryCodeAndName;

struct ListOfCountryNamesByCodeResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCountryCodeAndName ListOfCountryNamesByCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCountryNamesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByCodeResponse& obj);

ListOfCountryNamesByCodeResponse ListOfCountryNamesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCountryNamesByCodeResponseToXml(
    const ListOfCountryNamesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
