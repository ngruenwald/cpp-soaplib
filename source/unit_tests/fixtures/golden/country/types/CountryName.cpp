// CountryName

#include "CountryName.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CountryNameFromXml(
    const soaplib::xml::Node& objNode,
    CountryName& obj)
{
    obj.SCountryISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

CountryName CountryNameFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryName obj;
    CountryNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryName>();
    CountryNameFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryNameToXml(
    const CountryName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void CountryNameToXml(
    const CountryName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CountryName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryNameToXml(obj, doc, objNode);
    }
    else
    {
        _CountryNameToXml(obj, doc, parentNode);
    }
}

void CountryName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CountryName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryNameToXml(*this, doc, node);
}

} // namespace country
