// CurrencyNameResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CurrencyNameResponse
    : soaplib::SoapBaseType
{
    soaplib::String CurrencyNameResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CurrencyNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    CurrencyNameResponse& obj);

CurrencyNameResponse CurrencyNameResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CurrencyNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CurrencyNameResponseToXml(
    const CurrencyNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
