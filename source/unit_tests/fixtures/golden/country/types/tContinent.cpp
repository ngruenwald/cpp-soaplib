// tContinent

#include "tContinent.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void tContinentFromXml(
    const soaplib::xml::Node& objNode,
    tContinent& obj)
{
    obj.SCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCode", StringFromXml);
    obj.SName = soaplib::GetMandatory<soaplib::String>(objNode, "sName", StringFromXml);
}

tContinent tContinentFromXml(
    const soaplib::xml::Node& objNode)
{
    tContinent obj;
    tContinentFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tContinentPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tContinent>();
    tContinentFromXml(objNode, *obj.get());
    return obj;
}

static void _tContinentToXml(
    const tContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCode);
    }
    {
        auto pn = soaplib::AddChild(doc, objNode, "sName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SName);
    }
}

void tContinentToXml(
    const tContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "tContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tContinentToXml(obj, doc, objNode);
    }
    else
    {
        _tContinentToXml(obj, doc, parentNode);
    }
}

void tContinent::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "tContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tContinentToXml(*this, doc, node);
}

} // namespace country
