// tCurrency

#include "tCurrency.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void tCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    tCurrency& obj)
{
    obj.SISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sISOCode", StringFromXml);
    obj.SName = soaplib::getMandatory<soaplib::String>(objNode, "sName", StringFromXml);
}

tCurrency tCurrencyFromXml(
    const soaplib::xml::Node& objNode)
{
    tCurrency obj;
    tCurrencyFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tCurrency>();
    tCurrencyFromXml(objNode, *obj.get());
    return obj;
}

static void _tCurrencyToXml(
    const tCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SISOCode);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SName);
    }
}

void tCurrencyToXml(
    const tCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "tCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tCurrencyToXml(obj, doc, objNode);
    }
    else
    {
        _tCurrencyToXml(obj, doc, parentNode);
    }
}

void tCurrency::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "tCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tCurrencyToXml(*this, doc, node);
}

} // namespace country
