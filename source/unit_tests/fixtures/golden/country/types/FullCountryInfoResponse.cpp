// FullCountryInfoResponse

#include "FullCountryInfoResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tCountryInfo.hpp"

namespace country {

void FullCountryInfoResponseFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoResponse& obj)
{
    obj.FullCountryInfoResult = soaplib::GetMandatory<tCountryInfo>(objNode, "FullCountryInfoResult", tCountryInfoFromXml);
}

FullCountryInfoResponse FullCountryInfoResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    FullCountryInfoResponse obj;
    FullCountryInfoResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<FullCountryInfoResponse>();
    FullCountryInfoResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _FullCountryInfoResponseToXml(
    const FullCountryInfoResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "FullCountryInfoResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCountryInfoToXml(obj.FullCountryInfoResult, doc, pn, false);
    }
}

void FullCountryInfoResponseToXml(
    const FullCountryInfoResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "FullCountryInfoResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _FullCountryInfoResponseToXml(obj, doc, objNode);
    }
    else
    {
        _FullCountryInfoResponseToXml(obj, doc, parentNode);
    }
}

void FullCountryInfoResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "FullCountryInfoResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _FullCountryInfoResponseToXml(*this, doc, node);
}

} // namespace country
