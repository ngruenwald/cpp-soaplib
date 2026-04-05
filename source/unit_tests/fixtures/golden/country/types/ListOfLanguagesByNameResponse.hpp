// ListOfLanguagesByNameResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftLanguage.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfLanguagesByNameResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftLanguage ListOfLanguagesByNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfLanguagesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByNameResponse& obj);

ListOfLanguagesByNameResponse ListOfLanguagesByNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfLanguagesByNameResponseToXml(
    const ListOfLanguagesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
