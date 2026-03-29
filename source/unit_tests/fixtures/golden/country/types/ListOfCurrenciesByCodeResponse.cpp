// ListOfCurrenciesByCodeResponse

#include "ListOfCurrenciesByCodeResponse.hpp"

#include <soaplib/parseHelper.hpp>

#include "ArrayOftCurrency.hpp"

namespace country {

void ListOfCurrenciesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode,
    ListOfCurrenciesByCodeResponse& obj)
{
    obj.ListOfCurrenciesByCodeResult = soaplib::getMandatory<ArrayOftCurrency>(objNode, "ListOfCurrenciesByCodeResult", ArrayOftCurrencyFromXml);
}

ListOfCurrenciesByCodeResponse ListOfCurrenciesByCodeResponseFromXml(
    const soaplib::xml::Node& objNode)
{
    ListOfCurrenciesByCodeResponse obj;
    ListOfCurrenciesByCodeResponseFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ListOfCurrenciesByCodeResponsePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ListOfCurrenciesByCodeResponse>();
    ListOfCurrenciesByCodeResponseFromXml(objNode, *obj.get());
    return obj;
}

static void _ListOfCurrenciesByCodeResponseToXml(
    const ListOfCurrenciesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "ListOfCurrenciesByCodeResult", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        ArrayOftCurrencyToXml(obj.ListOfCurrenciesByCodeResult, doc, pn, false);
    }
}

void ListOfCurrenciesByCodeResponseToXml(
    const ListOfCurrenciesByCodeResponse& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ListOfCurrenciesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ListOfCurrenciesByCodeResponseToXml(obj, doc, objNode);
    }
    else
    {
        _ListOfCurrenciesByCodeResponseToXml(obj, doc, parentNode);
    }
}

void ListOfCurrenciesByCodeResponse::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ListOfCurrenciesByCodeResponse", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ListOfCurrenciesByCodeResponseToXml(*this, doc, node);
}

} // namespace country
