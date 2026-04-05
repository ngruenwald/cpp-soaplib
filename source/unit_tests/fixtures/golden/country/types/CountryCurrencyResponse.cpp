// CountryCurrencyResponse

#include "CountryCurrencyResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tCurrency.hpp"

namespace country {

void CountryCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryCurrencyResponse& obj)
{
    obj.CountryCurrencyResult = soaplib::GetMandatory<tCurrency>(objNode, "CountryCurrencyResult", tCurrencyFromXml);
}

CountryCurrencyResponse CountryCurrencyResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryCurrencyResponse obj;
    CountryCurrencyResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryCurrencyResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryCurrencyResponse>();
    CountryCurrencyResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryCurrencyResponseToXml(
    const CountryCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "CountryCurrencyResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCurrencyToXml(obj.CountryCurrencyResult, doc, pn, false);
    }
}

void CountryCurrencyResponseToXml(
    const CountryCurrencyResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryCurrencyResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryCurrencyResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountryCurrencyResponseToXml(obj, doc, parentNode);
    }
}

void CountryCurrencyResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryCurrencyResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryCurrencyResponseToXml(*this, doc, node);
}

} // namespace country
