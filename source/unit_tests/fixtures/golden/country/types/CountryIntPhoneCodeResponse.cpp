// CountryIntPhoneCodeResponse

#include "CountryIntPhoneCodeResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CountryIntPhoneCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryIntPhoneCodeResponse& obj)
{
    obj.CountryIntPhoneCodeResult = soaplib::getMandatory<soaplib::String>(objNode, "CountryIntPhoneCodeResult", StringFromXml);
}

CountryIntPhoneCodeResponse CountryIntPhoneCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryIntPhoneCodeResponse obj;
    CountryIntPhoneCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryIntPhoneCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryIntPhoneCodeResponse>();
    CountryIntPhoneCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryIntPhoneCodeResponseToXml(
    const CountryIntPhoneCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "CountryIntPhoneCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CountryIntPhoneCodeResult);
    }
}

void CountryIntPhoneCodeResponseToXml(
    const CountryIntPhoneCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CountryIntPhoneCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryIntPhoneCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountryIntPhoneCodeResponseToXml(obj, doc, parentNode);
    }
}

void CountryIntPhoneCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CountryIntPhoneCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryIntPhoneCodeResponseToXml(*this, doc, node);
}

} // namespace country
