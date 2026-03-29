// ListOfLanguagesByNameResponse

#include "ListOfLanguagesByNameResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftLanguage.hpp"

namespace country {

void ListOfLanguagesByNameResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByNameResponse& obj)
{
    obj.ListOfLanguagesByNameResult = soaplib::getMandatory<ArrayOftLanguage>(objNode, "ListOfLanguagesByNameResult", ArrayOftLanguageFromXml);
}

ListOfLanguagesByNameResponse ListOfLanguagesByNameResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfLanguagesByNameResponse obj;
    ListOfLanguagesByNameResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByNameResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfLanguagesByNameResponse>();
    ListOfLanguagesByNameResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfLanguagesByNameResponseToXml(
    const ListOfLanguagesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfLanguagesByNameResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftLanguageToXml(obj.ListOfLanguagesByNameResult, doc, pn, false);
    }
}

void ListOfLanguagesByNameResponseToXml(
    const ListOfLanguagesByNameResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfLanguagesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfLanguagesByNameResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfLanguagesByNameResponseToXml(obj, doc, parentNode);
    }
}

void ListOfLanguagesByNameResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfLanguagesByNameResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfLanguagesByNameResponseToXml(*this, doc, node);
}

} // namespace country
