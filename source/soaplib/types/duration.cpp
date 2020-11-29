#include "duration.hpp"

#include <libxmlwrp.hpp>


namespace soaplib {


std::string Duration::ToString(const Duration& t)
{
	return {};
}

Duration Duration::FromString(const std::string& s)
{
	return {};
}

} // namespace soaplib


void DurationFromXml(
    const xml::Node& node,
    soaplib::Duration& obj)
{
    auto s = node.GetStringVal();
    obj = soaplib::Duration::FromString(s);
}

soaplib::Duration DurationFromXml(
    const xml::Node& node)
{
    auto obj = soaplib::Duration{};
    DurationFromXml(node, obj);
    return obj;
}

std::shared_ptr<soaplib::SoapBaseType> DurationPtrFromXml(
    const xml::Node& node)
{
    auto obj = std::make_shared<soaplib::Duration>();
    DurationFromXml(node, *obj.get());
    return std::static_pointer_cast<soaplib::SoapBaseType>(obj);
}

void DurationToXml(
    xml::Node& node,
    const soaplib::Duration& value)
{
    node.SetVal(soaplib::Duration::ToString(value));
}
