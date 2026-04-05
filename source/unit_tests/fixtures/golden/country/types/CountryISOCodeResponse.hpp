// CountryISOCodeResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryISOCodeResponse
    : soaplib::SoapBaseType
{
    soaplib::String CountryISOCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryISOCodeResponse& obj);

CountryISOCodeResponse CountryISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryISOCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryISOCodeResponseToXml(
    const CountryISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
