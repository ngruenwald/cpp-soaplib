// ListOfLanguagesByCodeResponse

#include "ListOfLanguagesByCodeResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftLanguage.hpp"

namespace country {

void ListOfLanguagesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfLanguagesByCodeResponse& obj)
{
    obj.ListOfLanguagesByCodeResult = soaplib::getMandatory<ArrayOftLanguage>(objNode, "ListOfLanguagesByCodeResult", ArrayOftLanguageFromXml);
}

ListOfLanguagesByCodeResponse ListOfLanguagesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfLanguagesByCodeResponse obj;
    ListOfLanguagesByCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfLanguagesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfLanguagesByCodeResponse>();
    ListOfLanguagesByCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfLanguagesByCodeResponseToXml(
    const ListOfLanguagesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfLanguagesByCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftLanguageToXml(obj.ListOfLanguagesByCodeResult, doc, pn, false);
    }
}

void ListOfLanguagesByCodeResponseToXml(
    const ListOfLanguagesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfLanguagesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfLanguagesByCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfLanguagesByCodeResponseToXml(obj, doc, parentNode);
    }
}

void ListOfLanguagesByCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfLanguagesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfLanguagesByCodeResponseToXml(*this, doc, node);
}

} // namespace country
