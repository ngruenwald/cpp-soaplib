// CapitalCity

#include "CapitalCity.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CapitalCityFromXml(
    const soaplib::xml::Node& objNode,
    CapitalCity& obj)
{
    obj.SCountryISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

CapitalCity CapitalCityFromXml(
    const soaplib::xml::Node& objNode)
{
    CapitalCity obj;
    CapitalCityFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CapitalCityPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CapitalCity>();
    CapitalCityFromXml(objNode, *obj.get());
    return obj;
}

static void _CapitalCityToXml(
    const CapitalCity& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void CapitalCityToXml(
    const CapitalCity& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CapitalCity", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CapitalCityToXml(obj, doc, objNode);
    }
    else
    {
        _CapitalCityToXml(obj, doc, parentNode);
    }
}

void CapitalCity::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CapitalCity", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CapitalCityToXml(*this, doc, node);
}

} // namespace country
