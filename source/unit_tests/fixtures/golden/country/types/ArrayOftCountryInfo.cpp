// ArrayOftCountryInfo

#include "ArrayOftCountryInfo.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tCountryInfo.hpp"

namespace country {

void ArrayOftCountryInfoFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCountryInfo& obj)
{
    obj.TCountryInfo = soaplib::GetMultiple<tCountryInfo>(objNode, "tCountryInfo", tCountryInfoFromXml);
}

ArrayOftCountryInfo ArrayOftCountryInfoFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftCountryInfo obj;
    ArrayOftCountryInfoFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCountryInfoPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftCountryInfo>();
    ArrayOftCountryInfoFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftCountryInfoToXml(
    const ArrayOftCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TCountryInfo)
    {
        auto pn = soaplib::AddChild(doc, objNode, "tCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCountryInfoToXml(entry, doc, pn, false);
    }
}

void ArrayOftCountryInfoToXml(
    const ArrayOftCountryInfo& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ArrayOftCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftCountryInfoToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftCountryInfoToXml(obj, doc, parentNode);
    }
}

void ArrayOftCountryInfo::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ArrayOftCountryInfo", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftCountryInfoToXml(*this, doc, node);
}

} // namespace country
