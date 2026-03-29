// FullCountryInfoAllCountries

#include "FullCountryInfoAllCountries.hpp"

#include <soaplib/parseHelper.hpp>


namespace country {

void FullCountryInfoAllCountriesFromXml(
    const soaplib::xml::Node& objNode,
    FullCountryInfoAllCountries& obj)
{
}

FullCountryInfoAllCountries FullCountryInfoAllCountriesFromXml(
    const soaplib::xml::Node& objNode)
{
    FullCountryInfoAllCountries obj;
    FullCountryInfoAllCountriesFromXml(objNode, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> FullCountryInfoAllCountriesPtrFromXml(
    const soaplib::xml::Node& objNode)
{
    auto obj = std::make_unique<FullCountryInfoAllCountries>();
    FullCountryInfoAllCountriesFromXml(objNode, *obj.get());
    return obj;
}

static void _FullCountryInfoAllCountriesToXml(
    const FullCountryInfoAllCountries& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& objNode)
{
}

void FullCountryInfoAllCountriesToXml(
    const FullCountryInfoAllCountries& obj,
    soaplib::xml::Document& doc,
    soaplib::xml::Node& parentNode,
    bool createNode)
{
    if (createNode)
    {
        auto objNode = soaplib::addChild(doc, parentNode, "FullCountryInfoAllCountries", "http://www.oorsprong.org/websamples.countryinfo", "tns");
        _FullCountryInfoAllCountriesToXml(obj, doc, objNode);
    }
    else
    {
        _FullCountryInfoAllCountriesToXml(obj, doc, parentNode);
    }
}

void FullCountryInfoAllCountries::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& node) const
{
    soaplib::setAnyTypeAttribute(doc, node, "FullCountryInfoAllCountries", "http://www.oorsprong.org/websamples.countryinfo", "tns");
    _FullCountryInfoAllCountriesToXml(*this, doc, node);
}

} // namespace country
