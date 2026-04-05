// CountryNameResponse

#include "CountryNameResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CountryNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    CountryNameResponse& obj)
{
    obj.CountryNameResult = soaplib::GetMandatory<soaplib::String>(objNode, "CountryNameResult", StringFromXml);
}

CountryNameResponse CountryNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    CountryNameResponse obj;
    CountryNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CountryNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CountryNameResponse>();
    CountryNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _CountryNameResponseToXml(
    const CountryNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "CountryNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.CountryNameResult);
    }
}

void CountryNameResponseToXml(
    const CountryNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CountryNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CountryNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _CountryNameResponseToXml(obj, doc, parentNode);
    }
}

void CountryNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CountryNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CountryNameResponseToXml(*this, doc, node);
}

} // namespace country
