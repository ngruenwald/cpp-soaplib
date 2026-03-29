// FullCountryInfo

#include "FullCountryInfo.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void FullCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfo& obj)
{
    obj.SCountryISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

FullCountryInfo FullCountryInfoFromXml(
    const soaplib::xml::Node& objNode)
{
    FullCountryInfo obj;
    FullCountryInfoFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<FullCountryInfo>();
    FullCountryInfoFromXml(objNode, *obj.get());
    return obj;
}

static void _FullCountryInfoToXml(
    const FullCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void FullCountryInfoToXml(
    const FullCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "FullCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _FullCountryInfoToXml(obj, doc, objNode);
    }
    else
    {
        _FullCountryInfoToXml(obj, doc, parentNode);
    }
}

void FullCountryInfo::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "FullCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _FullCountryInfoToXml(*this, doc, node);
}

} // namespace country
