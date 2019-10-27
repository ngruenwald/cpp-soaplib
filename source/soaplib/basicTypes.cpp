#include "basicTypes.hpp"

#include <libxmlwrp.hpp>

#ifdef WIN32
#include <string.h>
inline bool _strCaseCompare(
    const std::string& a,
    const std::string& b)
{
    return _stricmp(a.c_str(), b.c_str()) == 0;
}
#else
#include <strings.h>
inline bool _strCaseCompare(
    const std::string& a,
    const std::string& b)
{
    return strcasecmp(a.c_str(), b.c_str()) == 0;
}
#endif

//namespace soaplib {

template<typename Tvalue>
Tvalue _valueFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
    auto v = std::stoll(s);
    return static_cast<Tvalue>(v);
}

std::int8_t int8_tFromXml(
    const xml::Node& node)
{
	return _valueFromXml<std::int8_t>(node);
}

std::int16_t int16_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::int16_t>(node);
}

std::int32_t int32_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::int32_t>(node);
}

std::int64_t int64_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::int64_t>(node);
}


std::uint8_t uint8_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::uint8_t>(node);
}

std::uint16_t uint16_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::uint16_t>(node);
}

std::uint32_t uint32_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::uint32_t>(node);
}

std::uint64_t uint64_tFromXml(
    const xml::Node& node)
{
    return _valueFromXml<std::uint64_t>(node);
}

std::string stringFromXml(
    const xml::Node& node)
{
    return node.GetStringVal();
}


bool boolFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();

    if (_strCaseCompare(s, "true") || s == "1")
    {
        return true;
    }

    return false;
}


float floatFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stof(s);
}

double doubleFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
    return std::stod(s);
}


soaplib::Base64 Base64FromXml(
	const xml::Node& node)
{
	auto s = node.GetStringVal();
	return soaplib::Base64(base64_decode(s));
}

soaplib::Timestamp TimestampFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
    return soaplib::Timestamp::FromString(s);
}

soaplib::Duration DurationFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
    return soaplib::Duration::FromString(s);
}

soaplib::uuid uuidFromXml(
    const xml::Node& node)
{
    auto s = node.GetStringVal();
	soaplib::uuid u;
	u.from_string(s);
	return u;
}

//} // namespace soaplib
