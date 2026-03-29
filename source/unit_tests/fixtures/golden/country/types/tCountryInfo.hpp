// tCountryInfo
#pragma once

#include <memory>
#include <soaplib/basicTypes.hpp>

#include "ArrayOftLanguage.hpp"

namespace soaplib { namespace xml { class Document; } }
namespace soaplib { namespace xml { class Node; } }

namespace country {

struct tCountryInfo
    : soaplib::SoapBaseType
{
    soaplib::String SISOCode;
    soaplib::String SName;
    soaplib::String SCapitalCity;
    soaplib::String SPhoneCode;
    soaplib::String SContinentCode;
    soaplib::String SCurrencyISOCode;
    soaplib::String SCountryFlag;
    ::country::ArrayOftLanguage Languages;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& node) const override;
};

void tCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    tCountryInfo& obj);

tCountryInfo tCountryInfoFromXml(
    const soaplib::xml::Node& objNode);

std::unique_ptr<soaplib::SoapBaseType> tCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode);

void tCountryInfoToXml(
    const tCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode);

} // namespace country
