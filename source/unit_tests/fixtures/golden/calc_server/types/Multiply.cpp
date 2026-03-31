// Multiply

#include "Multiply.hpp"

#include <soaplib/parseHelper.hpp>


namespace calc {

void MultiplyFromXml(
    const soaplib::xml::Node& objNode,
    Multiply& obj)
{
    obj.IntA = soaplib::getMandatory<soaplib::Int32>(objNode, "intA", Int32FromXml);
    obj.IntB = soaplib::getMandatory<soaplib::Int32>(objNode, "intB", Int32FromXml);
}

Multiply MultiplyFromXml(
    const soaplib::xml::Node& objNode)
{
    Multiply obj;
    MultiplyFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> MultiplyPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<Multiply>();
    MultiplyFromXml(objNode, *obj.get());
    return obj;
}

static void _MultiplyToXml(
    const Multiply& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::addChild(doc, objNode, "intA", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.IntA);
    }
    {
        auto pn = soaplib::addChild(doc, objNode, "intB", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.IntB);
    }
}

void MultiplyToXml(
    const Multiply& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "Multiply", "http://tempuri.org/", "");
        _MultiplyToXml(obj, doc, objNode);
    }
    else
    {
        _MultiplyToXml(obj, doc, parentNode);
    }
}

void Multiply::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "Multiply", "http://tempuri.org/", "");
    _MultiplyToXml(*this, doc, node);
}

} // namespace calc
