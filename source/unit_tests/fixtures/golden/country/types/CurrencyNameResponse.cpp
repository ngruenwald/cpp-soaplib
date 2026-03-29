// CurrencyNameResponse

#include "CurrencyNameResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void CurrencyNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    CurrencyNameResponse& obj)
{
    obj.CurrencyNameResult = soaplib::getMandatory<soaplib::String>(objNode, "CurrencyNameResult", StringFromXml);
}

CurrencyNameResponse CurrencyNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CurrencyNameResponse obj;
    CurrencyNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CurrencyNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CurrencyNameResponse>();
    CurrencyNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CurrencyNameResponseToXml(
    const CurrencyNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "CurrencyNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CurrencyNameResult);
    }
}

void CurrencyNameResponseToXml(
    const CurrencyNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "CurrencyNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CurrencyNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CurrencyNameResponseToXml(obj, doc, parentNode);
    }
}

void CurrencyNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "CurrencyNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CurrencyNameResponseToXml(*this, doc, node);
}

} // namespace country
