// Divide

#include "Divide.hpp"

#include <soaplib/parseHelper.hpp>


namespace calc {

void DivideFromXml(
    const soaplib::xml::Node& objNode,
    Divide& obj)
{
    obj.IntA = soaplib::getMandatory<soaplib::Int32>(objNode, "intA", Int32FromXml);
    obj.IntB = soaplib::getMandatory<soaplib::Int32>(objNode, "intB", Int32FromXml);
}

Divide DivideFromXml(
    const soaplib::xml::Node& objNode)
{
    Divide obj;
    DivideFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> DividePtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<Divide>();
    DivideFromXml(objNode, *obj.get());
    return obj;
}

static void _DivideToXml(
    const Divide& obj,
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

void DivideToXml(
    const Divide& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "Divide", "http://tempuri.org/", "");
        _DivideToXml(obj, doc, objNode);
    }
    else
    {
        _DivideToXml(obj, doc, parentNode);
    }
}

void Divide::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "Divide", "http://tempuri.org/", "");
    _DivideToXml(*this, doc, node);
}

} // namespace calc
