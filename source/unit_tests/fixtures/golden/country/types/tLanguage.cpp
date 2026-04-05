// tLanguage

#include "tLanguage.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void tLanguageFromXml(
    const soaplib::xml::Node& objNode,
    tLanguage& obj)
{
    obj.SISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sISOCode", StringFromXml);
    obj.SName = soaplib::GetMandatory<soaplib::String>(objNode, "sName", StringFromXml);
}

tLanguage tLanguageFromXml(
    const soaplib::xml::Node& objNode)
{
    tLanguage obj;
    tLanguageFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tLanguagePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tLanguage>();
    tLanguageFromXml(objNode, *obj.get());
    return obj;
}

static void _tLanguageToXml(
    const tLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SISOCode);
    }
    {
        auto pn = soaplib::AddChild(doc, objNode, "sName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SName);
    }
}

void tLanguageToXml(
    const tLanguage& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "tLanguage", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tLanguageToXml(obj, doc, objNode);
    }
    else
    {
        _tLanguageToXml(obj, doc, parentNode);
    }
}

void tLanguage::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "tLanguage", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tLanguageToXml(*this, doc, node);
}

} // namespace country
