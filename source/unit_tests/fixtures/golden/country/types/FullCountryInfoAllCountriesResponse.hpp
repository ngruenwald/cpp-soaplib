// FullCountryInfoAllCountriesResponse
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>

#include "ArrayOftCountryInfo.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {
struct ArrayOftCountryInfo;

struct FullCountryInfoAllCountriesResponse
    : soaplib::SoapBaseType
{
    ::country::ArrayOftCountryInfo FullCountryInfoAllCountriesResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void FullCountryInfoAllCountriesResponseFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoAllCountriesResponse& obj);

FullCountryInfoAllCountriesResponse FullCountryInfoAllCountriesResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoAllCountriesResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void FullCountryInfoAllCountriesResponseToXml(
    const FullCountryInfoAllCountriesResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
