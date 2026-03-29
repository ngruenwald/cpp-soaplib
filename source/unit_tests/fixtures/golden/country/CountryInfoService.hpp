// CountryInfoService

#pragma once

#include <soaplib/soapService.hpp>

#include "types/ListOfContinentsByName.hpp"
#include "types/ListOfContinentsByNameResponse.hpp"
#include "types/ListOfContinentsByCode.hpp"
#include "types/ListOfContinentsByCodeResponse.hpp"
#include "types/ListOfCurrenciesByName.hpp"
#include "types/ListOfCurrenciesByNameResponse.hpp"
#include "types/ListOfCurrenciesByCode.hpp"
#include "types/ListOfCurrenciesByCodeResponse.hpp"
#include "types/CurrencyName.hpp"
#include "types/CurrencyNameResponse.hpp"
#include "types/ListOfCountryNamesByCode.hpp"
#include "types/ListOfCountryNamesByCodeResponse.hpp"
#include "types/ListOfCountryNamesByName.hpp"
#include "types/ListOfCountryNamesByNameResponse.hpp"
#include "types/ListOfCountryNamesGroupedByContinent.hpp"
#include "types/ListOfCountryNamesGroupedByContinentResponse.hpp"
#include "types/CountryName.hpp"
#include "types/CountryNameResponse.hpp"
#include "types/CountryISOCode.hpp"
#include "types/CountryISOCodeResponse.hpp"
#include "types/CapitalCity.hpp"
#include "types/CapitalCityResponse.hpp"
#include "types/CountryCurrency.hpp"
#include "types/CountryCurrencyResponse.hpp"
#include "types/CountryFlag.hpp"
#include "types/CountryFlagResponse.hpp"
#include "types/CountryIntPhoneCode.hpp"
#include "types/CountryIntPhoneCodeResponse.hpp"
#include "types/FullCountryInfo.hpp"
#include "types/FullCountryInfoResponse.hpp"
#include "types/FullCountryInfoAllCountries.hpp"
#include "types/FullCountryInfoAllCountriesResponse.hpp"
#include "types/CountriesUsingCurrency.hpp"
#include "types/CountriesUsingCurrencyResponse.hpp"
#include "types/ListOfLanguagesByName.hpp"
#include "types/ListOfLanguagesByNameResponse.hpp"
#include "types/ListOfLanguagesByCode.hpp"
#include "types/ListOfLanguagesByCodeResponse.hpp"
#include "types/LanguageName.hpp"
#include "types/LanguageNameResponse.hpp"
#include "types/LanguageISOCode.hpp"
#include "types/LanguageISOCodeResponse.hpp"

namespace country {

The "CountryInfoService" SOAP service
class CountryInfoService
    : public soaplib::SoapService
{
public:
    /// Constructs a new instance of the "CountryInfoService" service.
    /// @param[in]  serviceAddress  URL of the SOAP service.
    CountryInfoService(
        const std::string& serviceAddress);

    /// Destructs the service instance.
    ~CountryInfoService();

public:
    ::country::ListOfContinentsByNameResponse ListOfContinentsByName(
        const ::country::ListOfContinentsByName& input);

    ::country::ListOfContinentsByCodeResponse ListOfContinentsByCode(
        const ::country::ListOfContinentsByCode& input);

    ::country::ListOfCurrenciesByNameResponse ListOfCurrenciesByName(
        const ::country::ListOfCurrenciesByName& input);

    ::country::ListOfCurrenciesByCodeResponse ListOfCurrenciesByCode(
        const ::country::ListOfCurrenciesByCode& input);

    ::country::CurrencyNameResponse CurrencyName(
        const ::country::CurrencyName& input);

    ::country::ListOfCountryNamesByCodeResponse ListOfCountryNamesByCode(
        const ::country::ListOfCountryNamesByCode& input);

    ::country::ListOfCountryNamesByNameResponse ListOfCountryNamesByName(
        const ::country::ListOfCountryNamesByName& input);

    ::country::ListOfCountryNamesGroupedByContinentResponse ListOfCountryNamesGroupedByContinent(
        const ::country::ListOfCountryNamesGroupedByContinent& input);

    ::country::CountryNameResponse CountryName(
        const ::country::CountryName& input);

    ::country::CountryISOCodeResponse CountryISOCode(
        const ::country::CountryISOCode& input);

    ::country::CapitalCityResponse CapitalCity(
        const ::country::CapitalCity& input);

    ::country::CountryCurrencyResponse CountryCurrency(
        const ::country::CountryCurrency& input);

    ::country::CountryFlagResponse CountryFlag(
        const ::country::CountryFlag& input);

    ::country::CountryIntPhoneCodeResponse CountryIntPhoneCode(
        const ::country::CountryIntPhoneCode& input);

    ::country::FullCountryInfoResponse FullCountryInfo(
        const ::country::FullCountryInfo& input);

    ::country::FullCountryInfoAllCountriesResponse FullCountryInfoAllCountries(
        const ::country::FullCountryInfoAllCountries& input);

    ::country::CountriesUsingCurrencyResponse CountriesUsingCurrency(
        const ::country::CountriesUsingCurrency& input);

    ::country::ListOfLanguagesByNameResponse ListOfLanguagesByName(
        const ::country::ListOfLanguagesByName& input);

    ::country::ListOfLanguagesByCodeResponse ListOfLanguagesByCode(
        const ::country::ListOfLanguagesByCode& input);

    ::country::LanguageNameResponse LanguageName(
        const ::country::LanguageName& input);

    ::country::LanguageISOCodeResponse LanguageISOCode(
        const ::country::LanguageISOCode& input);

};

} // namespace country
