// CountriesUsingCurrency

#include "CountriesUsingCurrency.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CountriesUsingCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    CountriesUsingCurrency& obj)
{
    obj.SISOCurrencyCode = soaplib::getMandatory<soaplib::String>(objNode, "sISOCurrencyCode", StringFromXml);
}

CountriesUsingCurrency CountriesUsingCurrencyFromXml(
    const soaplib::xml::Node& objNode)
{
    CountriesUsingCurrency obj;
    CountriesUsingCurrencyFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountriesUsingCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountriesUsingCurrency>();
    CountriesUsingCurrencyFromXml(objNode, *obj.get());
    return obj;
}

static void _CountriesUsingCurrencyToXml(
    const CountriesUsingCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sISOCurrencyCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SISOCurrencyCode);
    }
}

void CountriesUsingCurrencyToXml(
    const CountriesUsingCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CountriesUsingCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountriesUsingCurrencyToXml(obj, doc, objNode);
    }
    else
    {
        _CountriesUsingCurrencyToXml(obj, doc, parentNode);
    }
}

void CountriesUsingCurrency::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CountriesUsingCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountriesUsingCurrencyToXml(*this, doc, node);
}

} // namespace country
