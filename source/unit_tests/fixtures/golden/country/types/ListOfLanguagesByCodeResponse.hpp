// ListOfLanguagesByCodeResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftLanguage.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfLanguagesByCodeResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftLanguage ListOfLanguagesByCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfLanguagesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByCodeResponse& obj);

ListOfLanguagesByCodeResponse ListOfLanguagesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfLanguagesByCodeResponseToXml(
    const ListOfLanguagesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
