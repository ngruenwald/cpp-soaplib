// ArrayOftCurrency

#include "ArrayOftCurrency.hpp"

#include <soaplib/ParseHelper.hpp>

#include "tCurrency.hpp"

namespace country {

void ArrayOftCurrencyFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftCurrency& obj)
{
    obj.TCurrency = soaplib::GetMultiple<tCurrency>(objNode, "tCurrency", tCurrencyFromXml);
}

ArrayOftCurrency ArrayOftCurrencyFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftCurrency obj;
    ArrayOftCurrencyFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftCurrencyPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftCurrency>();
    ArrayOftCurrencyFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftCurrencyToXml(
    const ArrayOftCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TCurrency)
    {
        auto pn = soaplib::AddChild(doc, objNode, "tCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tCurrencyToXml(entry, doc, pn, false);
    }
}

void ArrayOftCurrencyToXml(
    const ArrayOftCurrency& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "ArrayOftCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftCurrencyToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftCurrencyToXml(obj, doc, parentNode);
    }
}

void ArrayOftCurrency::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "ArrayOftCurrency", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftCurrencyToXml(*this, doc, node);
}

} // namespace country
