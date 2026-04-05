// CountryCurrency
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryCurrency
    : soaplib::SoapBaseType
{
    soaplib::String SCountryISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    CountryCurrency& obj);

CountryCurrency CountryCurrencyFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryCurrencyToXml(
    const CountryCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
