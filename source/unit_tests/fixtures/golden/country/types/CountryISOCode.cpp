// CountryISOCode

#include "CountryISOCode.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CountryISOCodeFromXml(
    const soaplib::xml::Node& objNode,
    CountryISOCode& obj)
{
    obj.SCountryName = soaplib::getMandatory<soaplib::String>(objNode, "sCountryName", StringFromXml);
}

CountryISOCode CountryISOCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryISOCode obj;
    CountryISOCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryISOCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryISOCode>();
    CountryISOCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryISOCodeToXml(
    const CountryISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sCountryName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryName);
    }
}

void CountryISOCodeToXml(
    const CountryISOCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryISOCodeToXml(obj, doc, objNode);
    }
    else
    {
        _CountryISOCodeToXml(obj, doc, parentNode);
    }
}

void CountryISOCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryISOCodeToXml(*this, doc, node);
}

} // namespace country
