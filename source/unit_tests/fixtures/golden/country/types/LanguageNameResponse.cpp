// LanguageNameResponse

#include "LanguageNameResponse.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void LanguageNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    LanguageNameResponse& obj)
{
    obj.LanguageNameResult = soaplib::getMandatory<soaplib::String>(objNode, "LanguageNameResult", StringFromXml);
}

LanguageNameResponse LanguageNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    LanguageNameResponse obj;
    LanguageNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> LanguageNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<LanguageNameResponse>();
    LanguageNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _LanguageNameResponseToXml(
    const LanguageNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "LanguageNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.LanguageNameResult);
    }
}

void LanguageNameResponseToXml(
    const LanguageNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "LanguageNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _LanguageNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _LanguageNameResponseToXml(obj, doc, parentNode);
    }
}

void LanguageNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "LanguageNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _LanguageNameResponseToXml(*this, doc, node);
}

} // namespace country
