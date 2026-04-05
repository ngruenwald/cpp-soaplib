// CountryFlagResponse

#include "CountryFlagResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryFlagResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryFlagResponse& obj)
{
    obj.CountryFlagResult = soaplib::GetMandatory<soaplib::String>(objNode, "CountryFlagResult", StringFromXml);
}

CountryFlagResponse CountryFlagResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryFlagResponse obj;
    CountryFlagResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryFlagResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryFlagResponse>();
    CountryFlagResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryFlagResponseToXml(
    const CountryFlagResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "CountryFlagResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CountryFlagResult);
    }
}

void CountryFlagResponseToXml(
    const CountryFlagResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryFlagResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryFlagResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountryFlagResponseToXml(obj, doc, parentNode);
    }
}

void CountryFlagResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryFlagResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryFlagResponseToXml(*this, doc, node);
}

} // namespace country
