// CountryInfoService

#include "CountryInfoService.hpp"

#include <soaplib/ParseHelper.hpp>

namespace country {

using namespace ::soaplib;

CountryInfoService::CountryInfoService(
    const std::string& serviceAddress,
    const soaplib::HttpConfig& config)
    : soaplib::SoapService(serviceAddress, "", config)
{
    SetSoapVersion(soaplib::SoapVersion::Soap11);
    RegisterUnderstoodHeader("Action", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("To", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("MessageID", "http://www.w3.org/2005/08/addressing");
    RegisterUnderstoodHeader("ReplyTo", "http://www.w3.org/2005/08/addressing");
}

CountryInfoService::~CountryInfoService()
{
}

::country::ListOfContinentsByNameResponse CountryInfoService::ListOfContinentsByName(
    const ::country::ListOfContinentsByName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfContinentsByNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfContinentsByNameResponse");

    return ::country::ListOfContinentsByNameResponseFromXml(operation);
}

::country::ListOfContinentsByCodeResponse CountryInfoService::ListOfContinentsByCode(
    const ::country::ListOfContinentsByCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfContinentsByCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfContinentsByCodeResponse");

    return ::country::ListOfContinentsByCodeResponseFromXml(operation);
}

::country::ListOfCurrenciesByNameResponse CountryInfoService::ListOfCurrenciesByName(
    const ::country::ListOfCurrenciesByName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfCurrenciesByNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfCurrenciesByNameResponse");

    return ::country::ListOfCurrenciesByNameResponseFromXml(operation);
}

::country::ListOfCurrenciesByCodeResponse CountryInfoService::ListOfCurrenciesByCode(
    const ::country::ListOfCurrenciesByCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfCurrenciesByCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfCurrenciesByCodeResponse");

    return ::country::ListOfCurrenciesByCodeResponseFromXml(operation);
}

::country::CurrencyNameResponse CountryInfoService::CurrencyName(
    const ::country::CurrencyName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CurrencyNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CurrencyNameResponse");

    return ::country::CurrencyNameResponseFromXml(operation);
}

::country::ListOfCountryNamesByCodeResponse CountryInfoService::ListOfCountryNamesByCode(
    const ::country::ListOfCountryNamesByCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfCountryNamesByCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfCountryNamesByCodeResponse");

    return ::country::ListOfCountryNamesByCodeResponseFromXml(operation);
}

::country::ListOfCountryNamesByNameResponse CountryInfoService::ListOfCountryNamesByName(
    const ::country::ListOfCountryNamesByName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfCountryNamesByNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfCountryNamesByNameResponse");

    return ::country::ListOfCountryNamesByNameResponseFromXml(operation);
}

::country::ListOfCountryNamesGroupedByContinentResponse CountryInfoService::ListOfCountryNamesGroupedByContinent(
    const ::country::ListOfCountryNamesGroupedByContinent& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfCountryNamesGroupedByContinentToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfCountryNamesGroupedByContinentResponse");

    return ::country::ListOfCountryNamesGroupedByContinentResponseFromXml(operation);
}

::country::CountryNameResponse CountryInfoService::CountryName(
    const ::country::CountryName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountryNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountryNameResponse");

    return ::country::CountryNameResponseFromXml(operation);
}

::country::CountryISOCodeResponse CountryInfoService::CountryISOCode(
    const ::country::CountryISOCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountryISOCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountryISOCodeResponse");

    return ::country::CountryISOCodeResponseFromXml(operation);
}

::country::CapitalCityResponse CountryInfoService::CapitalCity(
    const ::country::CapitalCity& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CapitalCityToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CapitalCityResponse");

    return ::country::CapitalCityResponseFromXml(operation);
}

::country::CountryCurrencyResponse CountryInfoService::CountryCurrency(
    const ::country::CountryCurrency& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountryCurrencyToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountryCurrencyResponse");

    return ::country::CountryCurrencyResponseFromXml(operation);
}

::country::CountryFlagResponse CountryInfoService::CountryFlag(
    const ::country::CountryFlag& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountryFlagToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountryFlagResponse");

    return ::country::CountryFlagResponseFromXml(operation);
}

::country::CountryIntPhoneCodeResponse CountryInfoService::CountryIntPhoneCode(
    const ::country::CountryIntPhoneCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountryIntPhoneCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountryIntPhoneCodeResponse");

    return ::country::CountryIntPhoneCodeResponseFromXml(operation);
}

::country::FullCountryInfoResponse CountryInfoService::FullCountryInfo(
    const ::country::FullCountryInfo& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    FullCountryInfoToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("FullCountryInfoResponse");

    return ::country::FullCountryInfoResponseFromXml(operation);
}

::country::FullCountryInfoAllCountriesResponse CountryInfoService::FullCountryInfoAllCountries(
    const ::country::FullCountryInfoAllCountries& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    FullCountryInfoAllCountriesToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("FullCountryInfoAllCountriesResponse");

    return ::country::FullCountryInfoAllCountriesResponseFromXml(operation);
}

::country::CountriesUsingCurrencyResponse CountryInfoService::CountriesUsingCurrency(
    const ::country::CountriesUsingCurrency& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    CountriesUsingCurrencyToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("CountriesUsingCurrencyResponse");

    return ::country::CountriesUsingCurrencyResponseFromXml(operation);
}

::country::ListOfLanguagesByNameResponse CountryInfoService::ListOfLanguagesByName(
    const ::country::ListOfLanguagesByName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfLanguagesByNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfLanguagesByNameResponse");

    return ::country::ListOfLanguagesByNameResponseFromXml(operation);
}

::country::ListOfLanguagesByCodeResponse CountryInfoService::ListOfLanguagesByCode(
    const ::country::ListOfLanguagesByCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    ListOfLanguagesByCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("ListOfLanguagesByCodeResponse");

    return ::country::ListOfLanguagesByCodeResponseFromXml(operation);
}

::country::LanguageNameResponse CountryInfoService::LanguageName(
    const ::country::LanguageName& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    LanguageNameToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("LanguageNameResponse");

    return ::country::LanguageNameResponseFromXml(operation);
}

::country::LanguageISOCodeResponse CountryInfoService::LanguageISOCode(
    const ::country::LanguageISOCode& input)
{
    soaplib::xml::Document request;
    auto body = CreateEnvelope(request, "");
    LanguageISOCodeToXml(input, request, body, true);

    auto response = Call(request, "", soaplib::HttpMethod::Post);
    auto envelope = response->GetRootNode();
    auto operation = envelope.GetChild("Body").GetChild("LanguageISOCodeResponse");

    return ::country::LanguageISOCodeResponseFromXml(operation);
}


} // namespace country
