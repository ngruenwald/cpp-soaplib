// ArrayOftCurrency
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "tCurrency.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct ArrayOftCurrency
    : soaplib::SoapBaseType
{
    std::vector<::country::tCurrency> TCurrency;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void ArrayOftCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCurrency& obj);

ArrayOftCurrency ArrayOftCurrencyFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode);

void ArrayOftCurrencyToXml(
    const ArrayOftCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
