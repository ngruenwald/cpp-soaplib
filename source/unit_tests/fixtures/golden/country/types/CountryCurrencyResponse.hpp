// CountryCurrencyResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "tCurrency.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryCurrencyResponse
    : soaplib::SoapBaseType
{
    ::country::tCurrency CountryCurrencyResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryCurrencyResponse& obj);

CountryCurrencyResponse CountryCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryCurrencyResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryCurrencyResponseToXml(
    const CountryCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
