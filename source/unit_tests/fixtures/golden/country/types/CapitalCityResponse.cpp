// CapitalCityResponse

#include "CapitalCityResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CapitalCityResponseFromXml(
    const soaplib::xml::Node& objNode,
    CapitalCityResponse& obj)
{
    obj.CapitalCityResult = soaplib::getMandatory<soaplib::String>(objNode, "CapitalCityResult", StringFromXml);
}

CapitalCityResponse CapitalCityResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CapitalCityResponse obj;
    CapitalCityResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CapitalCityResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CapitalCityResponse>();
    CapitalCityResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CapitalCityResponseToXml(
    const CapitalCityResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "CapitalCityResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CapitalCityResult);
    }
}

void CapitalCityResponseToXml(
    const CapitalCityResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CapitalCityResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CapitalCityResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CapitalCityResponseToXml(obj, doc, parentNode);
    }
}

void CapitalCityResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CapitalCityResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CapitalCityResponseToXml(*this, doc, node);
}

} // namespace country
