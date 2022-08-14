#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <soaplib/types/soapBaseType.hpp>

namespace soaplib {

namespace xml { class Node; }

class Duration
    : public SoapBaseType
{
public:
    Duration() = default;

    Duration(
        int years, int months, int days,
        int hours, int minutes, int seconds)
        : years_(years), months_(months), days_(days)
        , hours_(hours), minutes_(minutes), seconds_(seconds)
    {
    }

    int Years() const { return years_; }
    int Months() const { return months_; }
    int Days() const { return days_; }
    int Hours() const { return hours_; }
    int Minutes() const { return minutes_; }
    int Seconds() const { return seconds_; }

public:
    static std::string ToString(const Duration& t);
    static Duration FromString(const std::string& s);

private:
    int years_ = 0;
    int months_ = 0;
    int days_ = 0;
    int hours_ = 0;
    int minutes_ = 0;
    int seconds_ = 0;

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
std::unique_ptr<soaplib::SoapBaseType> DurationPtrFromXml(const xml::Node& node);
void DurationToXml(xml::Node& node, const soaplib::Duration& value);
