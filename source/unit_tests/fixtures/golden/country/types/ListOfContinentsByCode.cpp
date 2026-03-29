// ListOfContinentsByCode

#include "ListOfContinentsByCode.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfContinentsByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfContinentsByCode& obj)
{
}

ListOfContinentsByCode ListOfContinentsByCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfContinentsByCode obj;
    ListOfContinentsByCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfContinentsByCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfContinentsByCode>();
    ListOfContinentsByCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfContinentsByCodeToXml(
    const ListOfContinentsByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfContinentsByCodeToXml(
    const ListOfContinentsByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfContinentsByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfContinentsByCodeToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfContinentsByCodeToXml(obj, doc, parentNode);
    }
}

void ListOfContinentsByCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfContinentsByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfContinentsByCodeToXml(*this, doc, node);
}

} // namespace country
