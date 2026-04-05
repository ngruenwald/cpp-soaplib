// CountryIntPhoneCode

#include "CountryIntPhoneCode.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryIntPhoneCodeFromXml(
    const soaplib::xml::Node& objNode,
    CountryIntPhoneCode& obj)
{
    obj.SCountryISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCountryISOCode", StringFromXml);
}

CountryIntPhoneCode CountryIntPhoneCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryIntPhoneCode obj;
    CountryIntPhoneCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryIntPhoneCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryIntPhoneCode>();
    CountryIntPhoneCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryIntPhoneCodeToXml(
    const CountryIntPhoneCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCountryISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryISOCode);
    }
}

void CountryIntPhoneCodeToXml(
    const CountryIntPhoneCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryIntPhoneCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryIntPhoneCodeToXml(obj, doc, objNode);
    }
    else
    {
        _CountryIntPhoneCodeToXml(obj, doc, parentNode);
    }
}

void CountryIntPhoneCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryIntPhoneCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryIntPhoneCodeToXml(*this, doc, node);
}

} // namespace country
