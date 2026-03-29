// tCountryInfo

#include "tCountryInfo.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftLanguage.hpp"

namespace country {

void tCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    tCountryInfo& obj)
{
    obj.SISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sISOCode", StringFromXml);
    obj.SName = soaplib::getMandatory<soaplib::String>(objNode, "sName", StringFromXml);
    obj.SCapitalCity = soaplib::getMandatory<soaplib::String>(objNode, "sCapitalCity", StringFromXml);
    obj.SPhoneCode = soaplib::getMandatory<soaplib::String>(objNode, "sPhoneCode", StringFromXml);
    obj.SContinentCode = soaplib::getMandatory<soaplib::String>(objNode, "sContinentCode", StringFromXml);
    obj.SCurrencyISOCode = soaplib::getMandatory<soaplib::String>(objNode, "sCurrencyISOCode", StringFromXml);
    obj.SCountryFlag = soaplib::getMandatory<soaplib::String>(objNode, "sCountryFlag", StringFromXml);
    obj.Languages = soaplib::getMandatory<ArrayOftLanguage>(objNode, "Languages", ArrayOftLanguageFromXml);
}

tCountryInfo tCountryInfoFromXml(
    const soaplib::xml::Node& objNode)
{
    tCountryInfo obj;
    tCountryInfoFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> tCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<tCountryInfo>();
    tCountryInfoFromXml(objNode, *obj.get());
    return obj;
}

static void _tCountryInfoToXml(
    const tCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "sISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SISOCode);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SName);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sCapitalCity", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCapitalCity);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sPhoneCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SPhoneCode);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sContinentCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SContinentCode);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sCurrencyISOCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCurrencyISOCode);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "sCountryFlag", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        StringToXml(pn, obj.SCountryFlag);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "Languages", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftLanguageToXml(obj.Languages, doc, pn, false);
    }
}

void tCountryInfoToXml(
    const tCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "tCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _tCountryInfoToXml(obj, doc, objNode);
    }
    else
    {
        _tCountryInfoToXml(obj, doc, parentNode);
    }
}

void tCountryInfo::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "tCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _tCountryInfoToXml(*this, doc, node);
}

} // namespace country
