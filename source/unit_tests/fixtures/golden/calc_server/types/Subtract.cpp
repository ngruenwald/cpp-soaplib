// Subtract

#include "Subtract.hpp"

#include <soaplib/ParseHelper.hpp>


namespace calc {

void SubtractFromXml(
    const soaplib::xml::Node& objNode,
    Subtract& obj)
{
    obj.IntA = soaplib::GetMandatory<soaplib::Int32>(objNode, "intA", Int32FromXml);
    obj.IntB = soaplib::GetMandatory<soaplib::Int32>(objNode, "intB", Int32FromXml);
}

Subtract SubtractFromXml(
    const soaplib::xml::Node& objNode)
{
    Subtract obj;
    SubtractFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> SubtractPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<Subtract>();
    SubtractFromXml(objNode, *obj.get());
    return obj;
}

static void _SubtractToXml(
    const Subtract& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    {
        auto pn = soaplib::AddChild(doc, objNode, "intA", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.IntA);
    }
    {
        auto pn = soaplib::AddChild(doc, objNode, "intB", "http://tempuri.org/", "");
        Int32ToXml(pn, obj.IntB);
    }
}

void SubtractToXml(
    const Subtract& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "Subtract", "http://tempuri.org/", "");
        _SubtractToXml(obj, doc, objNode);
    }
    else
    {
        _SubtractToXml(obj, doc, parentNode);
    }
}

void Subtract::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "Subtract", "http://tempuri.org/", "");
    _SubtractToXml(*this, doc, node);
}

} // namespace calc
