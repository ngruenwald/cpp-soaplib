// ListOfCurrenciesByName

#include "ListOfCurrenciesByName.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void ListOfCurrenciesByNameFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByName& obj)
{
}

ListOfCurrenciesByName ListOfCurrenciesByNameFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCurrenciesByName obj;
    ListOfCurrenciesByNameFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByNamePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCurrenciesByName>();
    ListOfCurrenciesByNameFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCurrenciesByNameToXml(
    const ListOfCurrenciesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfCurrenciesByNameToXml(
    const ListOfCurrenciesByName& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfCurrenciesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCurrenciesByNameToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCurrenciesByNameToXml(obj, doc, parentNode);
    }
}

void ListOfCurrenciesByName::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfCurrenciesByName", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCurrenciesByNameToXml(*this, doc, node);
}

} // namespace country
