#pragma once

#include <memory>
#include <optional>
#include <string>

#include <soaplib/types/soapBaseType.hpp>


namespace xml { class Node; }

namespace soaplib {

struct Timezone
{
    bool IsUTC = true;
    float Offset = 0.;
};

struct Date
    : soaplib::SoapBaseType
{
    Date() = default;
    Date(int yyyy, int mm, int dd)
        : Year{yyyy}, Month{mm}, Day{dd} {}
    Date(int yyyy, int mm, int dd, const Timezone& tz)
        : Year{yyyy}, Month{mm}, Day{dd}, Timezone{tz} {}

    int Year = 0;
    int Month = 0;
    int Day = 0;

    std::optional<struct Timezone> Timezone;
};

struct Time
    : soaplib::SoapBaseType
{
    Time() = default;
    Time(int hh, int mm, int ss, int ff = 0)
        : Hour{hh}, Minute{mm}, Second{ss}, Fraction{ff} {}
    Time(int hh, int mm, int ss, int ff, const Timezone& tz)
        : Hour{hh}, Minute{mm}, Second{ss}, Fraction{ff}, Timezone{tz} {}

    int Hour = 0;
    int Minute = 0;
    int Second = 0;
    int Fraction = 0;

    std::optional<struct Timezone> Timezone;
};

struct DateTime
    : soaplib::SoapBaseType
{
    DateTime() = default;
    DateTime(const Date& d, const Time& t)
        : Date{d}, Time{t} {}
    DateTime(const Date& d, const Time& t, const Timezone& tz)
        : Date{d}, Time{t}, Timezone{tz} {}

    struct Date Date;
    struct Time Time;

    std::optional<struct Timezone> Timezone;
};

struct Duration
    : soaplib::SoapBaseType
{
    Duration() = default;
    Duration(int yyyy, int mm, int dd, int hh, int mi, int ss, bool neg = false)
        : Years{yyyy}, Months{mm}, Days{dd}
        , Hours{hh}, Minutes{mi}, Seconds{ss}
        , IsNegative{neg} {}

    int Years = 0;
    int Months = 0;
    int Days = 0;

    int Hours = 0;
    int Minutes = 0;
    int Seconds = 0;

    bool IsNegative = false;
};

} // namespace soaplib

//
// string conversion
//

void from_string(const std::string& s, soaplib::Timezone& tz);
std::string to_string(const soaplib::Timezone& tz);
void to_string(std::string& s, const soaplib::Timezone& tz);

void from_string(const std::string& s, soaplib::Date& tz);
std::string to_string(const soaplib::Date& tz);
void to_string(std::string& s, const soaplib::Date& tz);

void from_string(const std::string& s, soaplib::Time& tz);
std::string to_string(const soaplib::Time& tz);
void to_string(std::string& s, const soaplib::Time& tz);

void from_string(const std::string& s, soaplib::DateTime& tz);
std::string to_string(const soaplib::DateTime& tz);
void to_string(std::string& s, const soaplib::DateTime& tz);

void from_string(const std::string& s, soaplib::Duration& tz);
std::string to_string(const soaplib::Duration& tz);
void to_string(std::string& s, const soaplib::Duration& tz);

//
// xml conversion
//

void DateFromXml(const xml::Node& n, soaplib::Date& d);
soaplib::Date DateFromXml(const xml::Node& n);
std::shared_ptr<soaplib::SoapBaseType> DatePtrFromXml(const xml::Node& n);
void DateToXml(xml::Node& n, const soaplib::Date& d);

void TimeFromXml(const xml::Node& n, soaplib::Time& d);
soaplib::Time TimeFromXml(const xml::Node& n);
std::shared_ptr<soaplib::SoapBaseType> TimePtrFromXml(const xml::Node& n);
void TimeToXml(xml::Node& n, const soaplib::Time& d);

void DateTimeFromXml(const xml::Node& n, soaplib::DateTime& d);
soaplib::DateTime DateTimeFromXml(const xml::Node& n);
std::shared_ptr<soaplib::SoapBaseType> DateTimePtrFromXml(const xml::Node& n);
void DateTimeToXml(xml::Node& n, const soaplib::DateTime& d);

void DurationFromXml(const xml::Node& n, soaplib::Duration& d);
soaplib::Duration DurationFromXml(const xml::Node& n);
std::shared_ptr<soaplib::SoapBaseType> DurationPtrFromXml(const xml::Node& n);
void DurationToXml(xml::Node& n, const soaplib::Duration& d);
