// ListOfLanguagesByCode
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfLanguagesByCode
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfLanguagesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByCode& obj);

ListOfLanguagesByCode ListOfLanguagesByCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfLanguagesByCodeToXml(
    const ListOfLanguagesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
