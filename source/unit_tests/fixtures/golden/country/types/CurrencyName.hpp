// CurrencyName
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CurrencyName
    : soaplib::SoapBaseType
{
    soaplib::String SCurrencyISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CurrencyNameFromXml(
    const soaplib::xml::Node& objNode,
    CurrencyName& obj);

CurrencyName CurrencyNameFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CurrencyNamePtrFromXml(
    const soaplib::xml::Node& objNode);

void CurrencyNameToXml(
    const CurrencyName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
