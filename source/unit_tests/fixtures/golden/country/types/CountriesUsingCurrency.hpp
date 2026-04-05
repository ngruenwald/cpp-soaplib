// CountriesUsingCurrency
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountriesUsingCurrency
    : soaplib::SoapBaseType
{
    soaplib::String SISOCurrencyCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountriesUsingCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    CountriesUsingCurrency& obj);

CountriesUsingCurrency CountriesUsingCurrencyFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountriesUsingCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode);

void CountriesUsingCurrencyToXml(
    const CountriesUsingCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
