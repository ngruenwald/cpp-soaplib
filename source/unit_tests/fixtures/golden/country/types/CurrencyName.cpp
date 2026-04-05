// CurrencyName

#include "CurrencyName.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void CurrencyNameFromXml(
    const soaplib::xml::Node& objNode,
    CurrencyName& obj)
{
    obj.SCurrencyISOCode = soaplib::GetMandatory<soaplib::String>(objNode, "sCurrencyISOCode", StringFromXml);
}

CurrencyName CurrencyNameFromXml(
    const soaplib::xml::Node& objNode)
{
    CurrencyName obj;
    CurrencyNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> CurrencyNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<CurrencyName>();
    CurrencyNameFromXml(objNode, *obj.get());
    return obj;
}

static void _CurrencyNameToXml(
    const CurrencyName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "sCurrencyISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCurrencyISOCode);
    }
}

void CurrencyNameToXml(
    const CurrencyName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "CurrencyName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _CurrencyNameToXml(obj, doc, objNode);
    }
    else
    {
        _CurrencyNameToXml(obj, doc, parentNode);
    }
}

void CurrencyName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "CurrencyName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _CurrencyNameToXml(*this, doc, node);
}

} // namespace country
