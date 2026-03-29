// tCountryCodeAndName

#include "tCountryCodeAndName.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void tCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode,
    tCountryCodeAndName& obj)
{
    obj.SISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sISOCode", StringFromXml);
    obj.SName = soaplib::getMandatory<soaplib::String>(objNode, "sName", StringFromXml);
}

tCountryCodeAndName tCountryCodeAndNameFromXml(
    const soaplib::xml::Node& objNode)
{
    tCountryCodeAndName obj;
    tCountryCodeAndNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tCountryCodeAndNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tCountryCodeAndName>();
    tCountryCodeAndNameFromXml(objNode, *obj.get());
    return obj;
}

static void _tCountryCodeAndNameToXml(
    const tCountryCodeAndName& obj,
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

void tCountryCodeAndNameToXml(
    const tCountryCodeAndName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "tCountryCodeAndName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tCountryCodeAndNameToXml(obj, doc, objNode);
    }
    else
    {
        _tCountryCodeAndNameToXml(obj, doc, parentNode);
    }
}

void tCountryCodeAndName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "tCountryCodeAndName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tCountryCodeAndNameToXml(*this, doc, node);
}

} // namespace country
