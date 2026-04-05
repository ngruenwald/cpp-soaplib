// CountryCurrency

#include "CountryCurrency.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    CountryCurrency& obj)
{
    obj.SCountryISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

CountryCurrency CountryCurrencyFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryCurrency obj;
    CountryCurrencyFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryCurrency>();
    CountryCurrencyFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryCurrencyToXml(
    const CountryCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void CountryCurrencyToXml(
    const CountryCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryCurrencyToXml(obj, doc, objNode);
    }
    else
    {
        _CountryCurrencyToXml(obj, doc, parentNode);
    }
}

void CountryCurrency::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryCurrencyToXml(*this, doc, node);
}

} // namespace country
