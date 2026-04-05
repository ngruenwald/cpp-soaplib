// FullCountryInfoAllCountriesResponse

#include "FullCountryInfoAllCountriesResponse.hpp"

#include <soaplib/ParseHelper.hpp>

#include "ArrayOftCountryInfo.hpp"

namespace country {

void FullCountryInfoAllCountriesResponseFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoAllCountriesResponse& obj)
{
    obj.FullCountryInfoAllCountriesResult = soaplib::GetMandatory<ArrayOftCountryInfo>(objNode, "FullCountryInfoAllCountriesResult", ArrayOftCountryInfoFromXml);
}

FullCountryInfoAllCountriesResponse FullCountryInfoAllCountriesResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    FullCountryInfoAllCountriesResponse obj;
    FullCountryInfoAllCountriesResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoAllCountriesResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<FullCountryInfoAllCountriesResponse>();
    FullCountryInfoAllCountriesResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _FullCountryInfoAllCountriesResponseToXml(
    const FullCountryInfoAllCountriesResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "FullCountryInfoAllCountriesResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCountryInfoToXml(obj.FullCountryInfoAllCountriesResult, doc, pn, false);
    }
}

void FullCountryInfoAllCountriesResponseToXml(
    const FullCountryInfoAllCountriesResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "FullCountryInfoAllCountriesResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _FullCountryInfoAllCountriesResponseToXml(obj, doc, objNode);
    }
    else
    {
        _FullCountryInfoAllCountriesResponseToXml(obj, doc, parentNode);
    }
}

void FullCountryInfoAllCountriesResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "FullCountryInfoAllCountriesResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _FullCountryInfoAllCountriesResponseToXml(*this, doc, node);
}

} // namespace country
