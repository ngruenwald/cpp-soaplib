// CountryISOCodeResponse

#include "CountryISOCodeResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryISOCodeResponse& obj)
{
    obj.CountryISOCodeResult = soaplib::GetMandatory<soaplib::String>(objNode, "CountryISOCodeResult", StringFromXml);
}

CountryISOCodeResponse CountryISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryISOCodeResponse obj;
    CountryISOCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryISOCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryISOCodeResponse>();
    CountryISOCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryISOCodeResponseToXml(
    const CountryISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "CountryISOCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CountryISOCodeResult);
    }
}

void CountryISOCodeResponseToXml(
    const CountryISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryISOCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryISOCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountryISOCodeResponseToXml(obj, doc, parentNode);
    }
}

void CountryISOCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryISOCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryISOCodeResponseToXml(*this, doc, node);
}

} // namespace country
