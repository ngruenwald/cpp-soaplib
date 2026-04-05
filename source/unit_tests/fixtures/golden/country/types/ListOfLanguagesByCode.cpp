// ListOfLanguagesByCode

#include "ListOfLanguagesByCode.hpp"

#include <soaplib/ParseHelper.hpp>


namespace country {

void ListOfLanguagesByCodeFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByCode& obj)
{
}

ListOfLanguagesByCode ListOfLanguagesByCodeFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfLanguagesByCode obj;
    ListOfLanguagesByCodeFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByCodePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfLanguagesByCode>();
    ListOfLanguagesByCodeFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfLanguagesByCodeToXml(
    const ListOfLanguagesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void ListOfLanguagesByCodeToXml(
    const ListOfLanguagesByCode& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ListOfLanguagesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfLanguagesByCodeToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfLanguagesByCodeToXml(obj, doc, parentNode);
    }
}

void ListOfLanguagesByCode::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ListOfLanguagesByCode", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfLanguagesByCodeToXml(*this, doc, node);
}

} // namespace country
