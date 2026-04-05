// tCurrency
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct tCurrency
    : soaplib::SoapBaseType
{
    soaplib::String SISOCode;
    soaplib::String SName;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    tCurrency& obj);

tCurrency tCurrencyFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode);

void tCurrencyToXml(
    const tCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
