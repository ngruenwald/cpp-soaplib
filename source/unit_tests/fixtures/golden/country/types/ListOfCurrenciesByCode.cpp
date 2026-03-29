// ListOfCurrenciesByCode

#include "ListOfCurrenciesByCode.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void ListOfCurrenciesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByCode& obj)
{
}

ListOfCurrenciesByCode ListOfCurrenciesByCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCurrenciesByCode obj;
    ListOfCurrenciesByCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCurrenciesByCode>();
    ListOfCurrenciesByCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCurrenciesByCodeToXml(
    const ListOfCurrenciesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfCurrenciesByCodeToXml(
    const ListOfCurrenciesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCurrenciesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCurrenciesByCodeToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCurrenciesByCodeToXml(obj, doc, parentNode);
    }
}

void ListOfCurrenciesByCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCurrenciesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCurrenciesByCodeToXml(*this, doc, node);
}

} // namespace country
