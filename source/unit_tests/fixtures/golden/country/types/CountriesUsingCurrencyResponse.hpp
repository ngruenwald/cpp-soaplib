// CountriesUsingCurrencyResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct ArrayOftCountryCodeAndName;

struct CountriesUsingCurrencyResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCountryCodeAndName CountriesUsingCurrencyResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountriesUsingCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountriesUsingCurrencyResponse& obj);

CountriesUsingCurrencyResponse CountriesUsingCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountriesUsingCurrencyResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountriesUsingCurrencyResponseToXml(
    const CountriesUsingCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
