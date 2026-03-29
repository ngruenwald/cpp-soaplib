// CountryIntPhoneCode
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryIntPhoneCode
    : soaplib::SoapBaseType
{
    soaplib::String SCountryISOCode;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryIntPhoneCodeFromXml(
    const soaplib::xml::Node& objNode,
    CountryIntPhoneCode& obj);

CountryIntPhoneCode CountryIntPhoneCodeFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryIntPhoneCodePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryIntPhoneCodeToXml(
    const CountryIntPhoneCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
