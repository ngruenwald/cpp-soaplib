// ListOfCurrenciesByCodeResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "ArrayOftCurrency.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCurrenciesByCodeResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCurrency ListOfCurrenciesByCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCurrenciesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByCodeResponse& obj);

ListOfCurrenciesByCodeResponse ListOfCurrenciesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCurrenciesByCodeResponseToXml(
    const ListOfCurrenciesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
