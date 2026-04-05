// LanguageISOCodeResponse

#include "LanguageISOCodeResponse.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void LanguageISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    LanguageISOCodeResponse& obj)
{
    obj.LanguageISOCodeResult = soaplib::GetMandatory<soaplib::String>(objNode, "LanguageISOCodeResult", StringFromXml);
}

LanguageISOCodeResponse LanguageISOCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    LanguageISOCodeResponse obj;
    LanguageISOCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> LanguageISOCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<LanguageISOCodeResponse>();
    LanguageISOCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _LanguageISOCodeResponseToXml(
    const LanguageISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "LanguageISOCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.LanguageISOCodeResult);
    }
}

void LanguageISOCodeResponseToXml(
    const LanguageISOCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "LanguageISOCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _LanguageISOCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _LanguageISOCodeResponseToXml(obj, doc, parentNode);
    }
}

void LanguageISOCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "LanguageISOCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _LanguageISOCodeResponseToXml(*this, doc, node);
}

} // namespace country
