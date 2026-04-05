// FullCountryInfoAllCountries
#pragma once

#include <memory>
#include <soaplib/BasicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct FullCountryInfoAllCountries
    : soaplib::SoapBaseType
{

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void FullCountryInfoAllCountriesFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoAllCountries& obj);

FullCountryInfoAllCountries FullCountryInfoAllCountriesFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoAllCountriesPtrFromXml(
    const soaplib::xml::Node& objNode);

void FullCountryInfoAllCountriesToXml(
    const FullCountryInfoAllCountries& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
