// ListOfCurrenciesByNameResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCurrency.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ListOfCurrenciesByNameResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCurrency ListOfCurrenciesByNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ListOfCurrenciesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByNameResponse& obj);

ListOfCurrenciesByNameResponse ListOfCurrenciesByNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void ListOfCurrenciesByNameResponseToXml(
    const ListOfCurrenciesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
