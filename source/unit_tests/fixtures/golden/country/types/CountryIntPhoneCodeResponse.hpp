// CountryIntPhoneCodeResponse
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>


namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct CountryIntPhoneCodeResponse
    : soaplib::SoapBaseType
{
    soaplib::String CountryIntPhoneCodeResult;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void CountryIntPhoneCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryIntPhoneCodeResponse& obj);

CountryIntPhoneCodeResponse CountryIntPhoneCodeResponseFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> CountryIntPhoneCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode);

void CountryIntPhoneCodeResponseToXml(
    const CountryIntPhoneCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
