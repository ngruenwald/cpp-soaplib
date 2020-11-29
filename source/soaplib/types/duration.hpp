#pragma once

#include <memory>
#include <string>
#include <soaplib/types/soapBaseType.hpp>


namespace xml { class Node; }

namespace soaplib {

class Duration
    : public SoapBaseType
{

public:
	static std::string ToString(const Duration& t);
	static Duration FromString(const std::string& s);

}; // Duration

} // namespace soaplib

/*
inline std::string to_string(const soaplib::Duration& t)
{
	return soaplib::Duration::ToString(t);
}

template<typename soaplib::Duration>
inline soaplib::Duration from_string(const std::string& s)
{
	return Timestamp::FromString(s);
}
*/

void DurationFromXml(const xml::Node& node, soaplib::Duration& obj);
soaplib::Duration DurationFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> DurationPtrFromXml(const xml::Node& node);
void DurationToXml(xml::Node& node, const soaplib::Duration& value);
