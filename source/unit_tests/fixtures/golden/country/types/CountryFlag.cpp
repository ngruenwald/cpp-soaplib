// CountryFlag

#include "CountryFlag.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryFlagFromXml(
    const soaplib::xml::Node& objNode,
    CountryFlag& obj)
{
    obj.SCountryISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

CountryFlag CountryFlagFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryFlag obj;
    CountryFlagFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryFlagPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryFlag>();
    CountryFlagFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryFlagToXml(
    const CountryFlag& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void CountryFlagToXml(
    const CountryFlag& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryFlag", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryFlagToXml(obj, doc, objNode);
    }
    else
    {
        _CountryFlagToXml(obj, doc, parentNode);
    }
}

void CountryFlag::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryFlag", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryFlagToXml(*this, doc, node);
}

} // namespace country
