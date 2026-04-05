// Add

#include "Add.hpp"

#include <soaplib/ParseHelper.hpp>


namespace calc {

void AddFromXml(
    const soaplib::xml::Node& objNode,
    Add& obj)
{
    obj.IntA = soaplib::GetMandatory<soaplib::Int32>(objNode, "intA", Int32FromXml);
    obj.IntB = soaplib::GetMandatory<soaplib::Int32>(objNode, "intB", Int32FromXml);
}

Add AddFromXml(
    const soaplib::xml::Node& objNode)
{
    Add obj;
    AddFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> AddPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<Add>();
    AddFromXml(objNode, *obj.get());
    return obj;
}

static void _AddToXml(
    const Add& obj,
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

void AddToXml(
    const Add& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::AddChild(doc, parentNode, "Add", "http://tempuri.org/", "");
        _AddToXml(obj, doc, objNode);
    }
    else
    {
        _AddToXml(obj, doc, parentNode);
    }
}

void Add::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::SetAnyTypeAttribute(doc, node, "Add", "http://tempuri.org/", "");
    _AddToXml(*this, doc, node);
}

} // namespace calc
