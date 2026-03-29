// CountriesUsingCurrencyResponse

#include "CountriesUsingCurrencyResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftCountryCodeAndName.hpp"

namespace country {

void CountriesUsingCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountriesUsingCurrencyResponse& obj)
{
    obj.CountriesUsingCurrencyResult = soaplib::getMandatory<ArrayOftCountryCodeAndName>(objNode, "CountriesUsingCurrencyResult", ArrayOftCountryCodeAndNameFromXml);
}

CountriesUsingCurrencyResponse CountriesUsingCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountriesUsingCurrencyResponse obj;
    CountriesUsingCurrencyResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountriesUsingCurrencyResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountriesUsingCurrencyResponse>();
    CountriesUsingCurrencyResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountriesUsingCurrencyResponseToXml(
    const CountriesUsingCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "CountriesUsingCurrencyResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryCodeAndNameToXml(obj.CountriesUsingCurrencyResult, doc, pn, false);
    }
}

void CountriesUsingCurrencyResponseToXml(
    const CountriesUsingCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CountriesUsingCurrencyResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountriesUsingCurrencyResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountriesUsingCurrencyResponseToXml(obj, doc, parentNode);
    }
}

void CountriesUsingCurrencyResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CountriesUsingCurrencyResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountriesUsingCurrencyResponseToXml(*this, doc, node);
}

} // namespace country
