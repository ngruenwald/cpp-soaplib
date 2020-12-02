#include "timestamp.hpp"

#include <chrono>
#include <cstring>
#include <iomanip>
#include <sstream>

#include <libxmlwrp.hpp>

namespace soaplib {

Timestamp Timestamp::Empty{};

Timestamp::Timestamp()
	: empty_(true)
	, ms_(0)
{
	tm_.tm_year = 0;
	tm_.tm_mon = 0;
	tm_.tm_mday = 1;
	tm_.tm_yday = 0;
	tm_.tm_hour = 0;
	tm_.tm_min = 0;
	tm_.tm_sec = 0;
	tm_.tm_isdst = 0;
}

Timestamp::Timestamp(const std::tm& tm, int ms)
	: empty_(false)
{
	memcpy(&tm_, &tm, sizeof(tm));
	ms_ = ms;
}

#ifdef WIN32
Timestamp::Timestamp(__time64_t tt, int ms)
	: empty_(false)
{
	gmtime_s(&tm_, &tt);
	ms_ = ms;
}
#else
Timestamp::Timestamp(time_t tt, int ms)
    : empty_(false)
{
	gmtime_r(&tt, &tm_);
	ms_ = ms;
}
#endif

Timestamp::Timestamp(const std::chrono::system_clock::time_point& tp, int ms)
	: Timestamp(std::chrono::system_clock::to_time_t(tp), ms)
{
}

Timestamp Timestamp::Now()
{
	return Timestamp{ std::chrono::system_clock::now() };
}

std::string Timestamp::ToString(const Timestamp& t)
{
	if (t.empty_)
	{
		return "0000-00-00T00:00:00.000Z";
	}

	std::ostringstream ss;
	ss << std::put_time(&t.tm_, "%FT%T.") << std::setw(3) << std::setfill('0') << t.ms_ << 'Z';
	return ss.str();
}

Timestamp Timestamp::FromString(const std::string& s)
{
	std::tm tm{ 0 };
	int ms = 0;

	auto idx = s.find('.');

	if (idx == std::string::npos)
	{
		std::istringstream ss(s);
		ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

		if (ss.fail())
		{
			return Timestamp::Empty;
		}
	}
	else
	{
		char zone;

		std::istringstream tss(s.substr(0, idx));
		tss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

		std::istringstream mss(s.substr(idx + 1));
		mss >> ms >> zone;

		if (tss.fail() || mss.fail() || zone != 'Z')
		{
			return Timestamp::Empty;
		}
	}

	if (tm.tm_mday == 0)
	{
		return Timestamp::Empty;
	}

	return Timestamp{ tm, ms };
}

} // namespace soaplib


void TimestampFromXml(
    const xml::Node& node,
    soaplib::Timestamp& obj)
{
    auto s = node.GetStringVal();
    obj = soaplib::Timestamp::FromString(s);
}

soaplib::Timestamp TimestampFromXml(
    const xml::Node& node)
{
    auto obj = soaplib::Timestamp{};
    TimestampFromXml(node, obj);
    return obj;
}

std::shared_ptr<soaplib::SoapBaseType> TimestampPtrFomXml(
    const xml::Node& node)
{
    auto obj = std::make_shared<soaplib::Timestamp>();
    TimestampFromXml(node, *obj.get());
    return std::static_pointer_cast<soaplib::SoapBaseType>(obj);
}

void TimestampToXml(
    xml::Node& node,
    const soaplib::Timestamp& value)
{
    node.SetVal(soaplib::Timestamp::ToString(value));
}
