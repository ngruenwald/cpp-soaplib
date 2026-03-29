// SoapLibAnyType

#include <string>
#include <map>
#include "SoapLibAnyType.hpp"

#include <soaplib/basicTypes.hpp>
#include <soaplib/parseHelper.hpp>

#include "ListOfContinentsByName.hpp"
#include "ListOfContinentsByNameResponse.hpp"
#include "ListOfContinentsByCode.hpp"
#include "ListOfContinentsByCodeResponse.hpp"
#include "ListOfCurrenciesByName.hpp"
#include "ListOfCurrenciesByNameResponse.hpp"
#include "ListOfCurrenciesByCode.hpp"
#include "ListOfCurrenciesByCodeResponse.hpp"
#include "CurrencyName.hpp"
#include "CurrencyNameResponse.hpp"
#include "ListOfCountryNamesByCode.hpp"
#include "ListOfCountryNamesByCodeResponse.hpp"
#include "ListOfCountryNamesByName.hpp"
#include "ListOfCountryNamesByNameResponse.hpp"
#include "ListOfCountryNamesGroupedByContinent.hpp"
#include "ListOfCountryNamesGroupedByContinentResponse.hpp"
#include "CountryName.hpp"
#include "CountryNameResponse.hpp"
#include "CountryISOCode.hpp"
#include "CountryISOCodeResponse.hpp"
#include "CapitalCity.hpp"
#include "CapitalCityResponse.hpp"
#include "CountryCurrency.hpp"
#include "CountryCurrencyResponse.hpp"
#include "CountryFlag.hpp"
#include "CountryFlagResponse.hpp"
#include "CountryIntPhoneCode.hpp"
#include "CountryIntPhoneCodeResponse.hpp"
#include "FullCountryInfo.hpp"
#include "FullCountryInfoResponse.hpp"
#include "FullCountryInfoAllCountries.hpp"
#include "FullCountryInfoAllCountriesResponse.hpp"
#include "CountriesUsingCurrency.hpp"
#include "CountriesUsingCurrencyResponse.hpp"
#include "ListOfLanguagesByName.hpp"
#include "ListOfLanguagesByNameResponse.hpp"
#include "ListOfLanguagesByCode.hpp"
#include "ListOfLanguagesByCodeResponse.hpp"
#include "LanguageName.hpp"
#include "LanguageNameResponse.hpp"
#include "LanguageISOCode.hpp"
#include "LanguageISOCodeResponse.hpp"
#include "tContinent.hpp"
#include "tCurrency.hpp"
#include "tCountryCodeAndName.hpp"
#include "tCountryCodeAndNameGroupedByContinent.hpp"
#include "tCountryInfo.hpp"
#include "tLanguage.hpp"
#include "ArrayOftCountryCodeAndName.hpp"
#include "ArrayOftLanguage.hpp"
#include "ArrayOftContinent.hpp"
#include "ArrayOftCurrency.hpp"
#include "ArrayOftCountryCodeAndNameGroupedByContinent.hpp"
#include "ArrayOftCountryInfo.hpp"

namespace country {

using namespace ::soaplib;

static std::map<std::string, std::unique_ptr<soaplib::SoapBaseType>(*)(const soaplib::xml::Node&)> typeMap =
{
    { "ListOfContinentsByName", ListOfContinentsByNamePtrFromXml },
    { "ListOfContinentsByNameResponse", ListOfContinentsByNameResponsePtrFromXml },
    { "ListOfContinentsByCode", ListOfContinentsByCodePtrFromXml },
    { "ListOfContinentsByCodeResponse", ListOfContinentsByCodeResponsePtrFromXml },
    { "ListOfCurrenciesByName", ListOfCurrenciesByNamePtrFromXml },
    { "ListOfCurrenciesByNameResponse", ListOfCurrenciesByNameResponsePtrFromXml },
    { "ListOfCurrenciesByCode", ListOfCurrenciesByCodePtrFromXml },
    { "ListOfCurrenciesByCodeResponse", ListOfCurrenciesByCodeResponsePtrFromXml },
    { "CurrencyName", CurrencyNamePtrFromXml },
    { "CurrencyNameResponse", CurrencyNameResponsePtrFromXml },
    { "ListOfCountryNamesByCode", ListOfCountryNamesByCodePtrFromXml },
    { "ListOfCountryNamesByCodeResponse", ListOfCountryNamesByCodeResponsePtrFromXml },
    { "ListOfCountryNamesByName", ListOfCountryNamesByNamePtrFromXml },
    { "ListOfCountryNamesByNameResponse", ListOfCountryNamesByNameResponsePtrFromXml },
    { "ListOfCountryNamesGroupedByContinent", ListOfCountryNamesGroupedByContinentPtrFromXml },
    { "ListOfCountryNamesGroupedByContinentResponse", ListOfCountryNamesGroupedByContinentResponsePtrFromXml },
    { "CountryName", CountryNamePtrFromXml },
    { "CountryNameResponse", CountryNameResponsePtrFromXml },
    { "CountryISOCode", CountryISOCodePtrFromXml },
    { "CountryISOCodeResponse", CountryISOCodeResponsePtrFromXml },
    { "CapitalCity", CapitalCityPtrFromXml },
    { "CapitalCityResponse", CapitalCityResponsePtrFromXml },
    { "CountryCurrency", CountryCurrencyPtrFromXml },
    { "CountryCurrencyResponse", CountryCurrencyResponsePtrFromXml },
    { "CountryFlag", CountryFlagPtrFromXml },
    { "CountryFlagResponse", CountryFlagResponsePtrFromXml },
    { "CountryIntPhoneCode", CountryIntPhoneCodePtrFromXml },
    { "CountryIntPhoneCodeResponse", CountryIntPhoneCodeResponsePtrFromXml },
    { "FullCountryInfo", FullCountryInfoPtrFromXml },
    { "FullCountryInfoResponse", FullCountryInfoResponsePtrFromXml },
    { "FullCountryInfoAllCountries", FullCountryInfoAllCountriesPtrFromXml },
    { "FullCountryInfoAllCountriesResponse", FullCountryInfoAllCountriesResponsePtrFromXml },
    { "CountriesUsingCurrency", CountriesUsingCurrencyPtrFromXml },
    { "CountriesUsingCurrencyResponse", CountriesUsingCurrencyResponsePtrFromXml },
    { "ListOfLanguagesByName", ListOfLanguagesByNamePtrFromXml },
    { "ListOfLanguagesByNameResponse", ListOfLanguagesByNameResponsePtrFromXml },
    { "ListOfLanguagesByCode", ListOfLanguagesByCodePtrFromXml },
    { "ListOfLanguagesByCodeResponse", ListOfLanguagesByCodeResponsePtrFromXml },
    { "LanguageName", LanguageNamePtrFromXml },
    { "LanguageNameResponse", LanguageNameResponsePtrFromXml },
    { "LanguageISOCode", LanguageISOCodePtrFromXml },
    { "LanguageISOCodeResponse", LanguageISOCodeResponsePtrFromXml },
    { "tContinent", tContinentPtrFromXml },
    { "tCurrency", tCurrencyPtrFromXml },
    { "tCountryCodeAndName", tCountryCodeAndNamePtrFromXml },
    { "tCountryCodeAndNameGroupedByContinent", tCountryCodeAndNameGroupedByContinentPtrFromXml },
    { "tCountryInfo", tCountryInfoPtrFromXml },
    { "tLanguage", tLanguagePtrFromXml },
    { "ArrayOftCountryCodeAndName", ArrayOftCountryCodeAndNamePtrFromXml },
    { "ArrayOftLanguage", ArrayOftLanguagePtrFromXml },
    { "ArrayOftContinent", ArrayOftContinentPtrFromXml },
    { "ArrayOftCurrency", ArrayOftCurrencyPtrFromXml },
    { "ArrayOftCountryCodeAndNameGroupedByContinent", ArrayOftCountryCodeAndNameGroupedByContinentPtrFromXml },
    { "ArrayOftCountryInfo", ArrayOftCountryInfoPtrFromXml },
};

static void addNamespace(
    soaplib::xml::Node& node,
    const char* prefix,
    const char* href)
{
    auto np = node.GetXmlNode();
    xmlSetNs(np, xmlNewNs(np, BAD_CAST href, BAD_CAST prefix));
}

void SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode,
    SoapLibAnyType& obj)
{
    auto type = objNode.GetStringProp("type");
    auto idx = type.find(':');
    if (idx != std::string::npos)
    {
        type = type.substr(idx + 1);
    }
    auto it = typeMap.find(type);
    if (it != typeMap.end())
    {
        obj.Value = it->second(objNode);
    }
}

SoapLibAnyType SoapLibAnyTypeFromXml(
    const soaplib::xml::Node& objNode)
{
    SoapLibAnyType obj;
    SoapLibAnyTypeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> SoapLibAnyTypePtrFromXml(
    const soaplib::xml::Node& node)
{
    return {}; // TODO
}

static void _SoapLibAnyTypeToXml(
    const SoapLibAnyType& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    if (obj.Value)
    {
        obj.Value->ToAnyXml(doc, objNode);
    }
}

void SoapLibAnyTypeToXml(
    const SoapLibAnyType& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "SoapLibAnyType", "", "");
        _SoapLibAnyTypeToXml(obj, doc, objNode);
    }
    else
    {
        _SoapLibAnyTypeToXml(obj, doc, parentNode);
    }
}

} // namespace country
