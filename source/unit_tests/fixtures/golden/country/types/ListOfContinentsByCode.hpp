// ListOfContinentsByCode
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfContinentsByCode
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfContinentsByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByCode& obj);

ListOfContinentsByCode ListOfContinentsByCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfContinentsByCodeToXml(
    const ListOfContinentsByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
