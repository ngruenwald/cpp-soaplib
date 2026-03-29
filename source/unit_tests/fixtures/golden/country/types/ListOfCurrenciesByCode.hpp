// ListOfCurrenciesByCode
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCurrenciesByCode
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCurrenciesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByCode& obj);

ListOfCurrenciesByCode ListOfCurrenciesByCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCurrenciesByCodeToXml(
    const ListOfCurrenciesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
