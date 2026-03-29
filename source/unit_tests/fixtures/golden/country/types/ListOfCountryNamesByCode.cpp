// ListOfCountryNamesByCode

#include "ListOfCountryNamesByCode.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfCountryNamesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCountryNamesByCode& obj)
{
}

ListOfCountryNamesByCode ListOfCountryNamesByCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCountryNamesByCode obj;
    ListOfCountryNamesByCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCountryNamesByCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCountryNamesByCode>();
    ListOfCountryNamesByCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCountryNamesByCodeToXml(
    const ListOfCountryNamesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfCountryNamesByCodeToXml(
    const ListOfCountryNamesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCountryNamesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCountryNamesByCodeToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCountryNamesByCodeToXml(obj, doc, parentNode);
    }
}

void ListOfCountryNamesByCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCountryNamesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCountryNamesByCodeToXml(*this, doc, node);
}

} // namespace country
