#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <soaplib/types/soapBaseType.hpp>


namespace xml { class Node; }

namespace soaplib {

class Timestamp
    : public SoapBaseType
{
public:
	static Timestamp Empty;

public:
	Timestamp();
	Timestamp(const std::tm& tm, int ms = 0);
#ifdef WIN32
	Timestamp(__time64_t tt, int ms = 0);
#else
	Timestamp(time_t tt, int ms = 0);
#endif
	Timestamp(const std::chrono::system_clock::time_point& tp, int ms = 0);

public:
	static Timestamp Now();

	static std::string ToString(const Timestamp& t);
	static Timestamp FromString(const std::string& s);

private:
	bool empty_;
	std::tm tm_;
	int ms_;
};

} // namespace soaplib

/*
inline std::string to_string(const soaplib::Timestamp& t)
{
	return soaplib::Timestamp::ToString(t);
}

template<typename soaplib::Timestamp>
inline soaplib::Timestamp from_string(const std::string& s)
{
	return Timestamp::FromString(s);
}
*/


void TimestampFromXml(const xml::Node& node, soaplib::Timestamp& obj);
soaplib::Timestamp TimestampFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> TimestampPtrFomXml(const xml::Node& node);
void TimestampToXml(xml::Node& node, const soaplib::Timestamp& value);
