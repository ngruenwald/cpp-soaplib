// ArrayOftContinent

#include "ArrayOftContinent.hpp"

#include <soaplib/parseHelper.hpp>

#include "tContinent.hpp"

namespace country {

void ArrayOftContinentFromXml(
    const soaplib::xml::Node& objNode,
    ArrayOftContinent& obj)
{
    obj.TContinent = soaplib::getMultiple<tContinent>(objNode, "tContinent", tContinentFromXml);
}

ArrayOftContinent ArrayOftContinentFromXml(
    const soaplib::xml::Node& objNode)
{
    ArrayOftContinent obj;
    ArrayOftContinentFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> ArrayOftContinentPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<ArrayOftContinent>();
    ArrayOftContinentFromXml(objNode, *obj.get());
    return obj;
}

static void _ArrayOftContinentToXml(
    const ArrayOftContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
    for (const auto& entry : obj.TContinent)
    {
        auto pn = soaplib::addChild(doc, objNode, "tContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        tContinentToXml(entry, doc, pn, false);
    }
}

void ArrayOftContinentToXml(
    const ArrayOftContinent& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "ArrayOftContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _ArrayOftContinentToXml(obj, doc, objNode);
    }
    else
    {
        _ArrayOftContinentToXml(obj, doc, parentNode);
    }
}

void ArrayOftContinent::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "ArrayOftContinent", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _ArrayOftContinentToXml(*this, doc, node);
}

} // namespace country
