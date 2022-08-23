#pragma once

#include <memory>
#include <optional>
#include <string>

#include <soaplib/types/soapBaseType.hpp>

namespace soaplib {

namespace xml { class Node; }

/// Holds timezone data
struct Timezone
{
    bool IsUTC = true;  ///< Indicates if this represents UTC
    float Offset = 0.;  ///< Timezone offset
};

/// Holds a date
struct Date
    : soaplib::SoapBaseType
{
    /// Default constructor
    Date() = default;

    /// Constructs a date object without timezone information
    /// @param[in] yyyy The year
    /// @param[in] mm The month
    /// @param[in] dd The day
    Date(int yyyy, int mm, int dd)
        : Year{yyyy}, Month{mm}, Day{dd} {}

    /// Constructs a date object with timezone information
    /// @param[in] yyyy The year
    /// @param[in] mm The month
    /// @param[in] dd The day
    /// @param[in] tz The timezone information
    Date(int yyyy, int mm, int dd, const Timezone& tz)
        : Year{yyyy}, Month{mm}, Day{dd}, Timezone{tz} {}

    /// Date to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

    int Year = 0;   ///< Year
    int Month = 0;  ///< Month of year
    int Day = 0;    ///< Day of month

    std::optional<struct Timezone> Timezone;    ///< Optional timezone
};

/// Holds a time point
struct Time
    : soaplib::SoapBaseType
{
    /// Default constructor
    Time() = default;

    /// Constructs a time object without timezone information
    /// @param[in] hh The hour
    /// @param[in] mm The minute
    /// @param[in] ss The second
    /// @param[in] ff The fraction
    Time(int hh, int mm, int ss, int ff = 0)
        : Hour{hh}, Minute{mm}, Second{ss}, Fraction{ff} {}

    /// Constructs a time object with timezone information
    /// @param[in] hh The hour
    /// @param[in] mm The minute
    /// @param[in] ss The second
    /// @param[in] ff The fraction
    /// @param[in] tz The timezone
    Time(int hh, int mm, int ss, int ff, const Timezone& tz)
        : Hour{hh}, Minute{mm}, Second{ss}, Fraction{ff}, Timezone{tz} {}

    /// Time to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

    int Hour = 0;       ///< Hour of day
    int Minute = 0;     ///< Minute in hour
    int Second = 0;     ///< Second in minute
    int Fraction = 0;   ///< Fraction of second

    std::optional<struct Timezone> Timezone;    ///< Optional timezone
};

/// Holds date and time
struct DateTime
    : soaplib::SoapBaseType
{
    /// Default constructor
    DateTime() = default;

    /// Constructs a date-time object without timezone
    /// @param[in] d The date
    /// @param[in] t The time
    DateTime(const Date& d, const Time& t)
        : Date{d}, Time{t} {}

    /// Constructs a date-time object with timezone
    /// @param[in] d The date
    /// @param[in] t The time
    /// @param[in] tz The timezone
    DateTime(const Date& d, const Time& t, const Timezone& tz)
        : Date{d}, Time{t}, Timezone{tz} {}

    /// Date and Time to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

    struct Date Date;   ///< Holds the date information
    struct Time Time;   ///< Holds the time information

    std::optional<struct Timezone> Timezone;    ///< Optional timezone
};

/// Holds a duration
struct Duration
    : soaplib::SoapBaseType
{
    /// Default constructor
    Duration() = default;

    /// Constructs a duration object
    /// @param[in] yyyy Years
    /// @param[in] mm Months
    /// @param[in] dd Days
    /// @param[in] hh Hours
    /// @param[in] mi Minutes
    /// @param[in] ss Seconds
    /// @param[in] neg Indicate negative duration
    Duration(int yyyy, int mm, int dd, int hh, int mi, int ss, bool neg = false)
        : Years{yyyy}, Months{mm}, Days{dd}
        , Hours{hh}, Minutes{mi}, Seconds{ss}
        , IsNegative{neg} {}

    /// Duration to XML
    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

    int Years = 0;      ///< Number of years
    int Months = 0;     ///< Number of months
    int Days = 0;       ///< Number of days

    int Hours = 0;      ///< Number of hours
    int Minutes = 0;    ///< Number of minutes
    int Seconds = 0;    ///< Number of seconds

    bool IsNegative = false;    ///< Indicates if the duration is negative
};

} // namespace soaplib

//
// string conversion
//

/// Parse timezone from string
/// @param[in] s Input string
/// @param[out] tz Timezone information
void from_string(const std::string& s, soaplib::Timezone& tz);

/// Serialize timezone to string
/// @param[in] tz Timezone information
/// @returns A string containing the timezone information
std::string to_string(const soaplib::Timezone& tz);

/// Serialize timezone to string
/// @param[out] s Output string
/// @param[in] tz Timezone information
void to_string(std::string& s, const soaplib::Timezone& tz);

/// Parse date from string
/// @param[in] s Input string
/// @param[out] d Date information
void from_string(const std::string& s, soaplib::Date& d);

/// Serialize date to string
/// @param[in] d Date information
/// @returns A string containing the date information
std::string to_string(const soaplib::Date& d);

/// Serialize date to string
/// @param[out] s Output string
/// @param[in] d Date information
void to_string(std::string& s, const soaplib::Date& d);

/// Parse time from string
/// @param[in] s Input string
/// @param[out] t Time information
void from_string(const std::string& s, soaplib::Time& t);

/// Serialize time to string
/// @param[in] t Time information
/// @returns A string containing the time information
std::string to_string(const soaplib::Time& t);

/// Serialize time to string
/// @param[out] s Output string
/// @param[in] t Time information
void to_string(std::string& s, const soaplib::Time& t);

/// Parse date-time from string
/// @param[in] s Input string
/// @param[out] dt DateTime information
void from_string(const std::string& s, soaplib::DateTime& dt);

/// Serialize date-time to string
/// @param[in] dt DateTime information
/// @returns A string containing the date-time information
std::string to_string(const soaplib::DateTime& dt);

/// Serialize date-time to string
/// @param[out] s Output string
/// @param[in] dt DateTime information
void to_string(std::string& s, const soaplib::DateTime& dt);

/// Parse duration from string
/// @param[in] s Input string
/// @param[out] du Duration information
void from_string(const std::string& s, soaplib::Duration& du);

/// Serialize duration to string
/// @param[in] du Duration information
/// @returns A string containing the duration information
std::string to_string(const soaplib::Duration& du);

/// Serialize duration to string
/// @param[out] s Output string
/// @param[in] du Duration information
void to_string(std::string& s, const soaplib::Duration& du);

//
// xml conversion
//

/// Extract date from XML
/// @param[in] n XML node
/// @param[out] d Date information
void DateFromXml(const soaplib::xml::Node& n, soaplib::Date& d);

/// Extract date from XML
/// @param[in] n XML node
/// @returns Date information
soaplib::Date DateFromXml(const soaplib::xml::Node& n);

/// Extract date from XML, as pointer type
/// @param[in] n XML node
/// @returns Pointer holding the date information
std::unique_ptr<soaplib::SoapBaseType> DatePtrFromXml(const soaplib::xml::Node& n);

/// Write date to XML
/// @param[in] n XML node
/// @param[in] d Date information
void DateToXml(soaplib::xml::Node& n, const soaplib::Date& d);

/// Extract time from XML
/// @param[in] n XML node
/// @param[out] t Time information
void TimeFromXml(const soaplib::xml::Node& n, soaplib::Time& t);

/// Extract time from XML
/// @param[in] n XML node
/// @returns Time information
soaplib::Time TimeFromXml(const soaplib::xml::Node& n);

/// Extract time from XML, as pointer type
/// @param[in] n XML node
/// @returns Pointer holding the time information
std::unique_ptr<soaplib::SoapBaseType> TimePtrFromXml(const soaplib::xml::Node& n);

/// Write time to XML
/// @param[in] n XML node
/// @param[in] t Time information
void TimeToXml(soaplib::xml::Node& n, const soaplib::Time& t);

/// Extract date-time from XML
/// @param[in] n XML node
/// @param[out] dt Time information
void DateTimeFromXml(const soaplib::xml::Node& n, soaplib::DateTime& dt);

/// Extract date-time from XML
/// @param[in] n XML node
/// @returns Date-time information
soaplib::DateTime DateTimeFromXml(const soaplib::xml::Node& n);

/// Extract date-time from XML, as pointer type
/// @param[in] n XML node
/// @returns Pointer holding the date-time information
std::unique_ptr<soaplib::SoapBaseType> DateTimePtrFromXml(const soaplib::xml::Node& n);

/// Write date-time to XML
/// @param[in] n XML node
/// @param[in] dt Date-time information
void DateTimeToXml(soaplib::xml::Node& n, const soaplib::DateTime& dt);

/// Extract duration from XML
/// @param[in] n XML node
/// @param[out] du Duration information
void DurationFromXml(const soaplib::xml::Node& n, soaplib::Duration& du);

/// Extract duration from XML
/// @param[in] n XML node
/// @returns Duration information
soaplib::Duration DurationFromXml(const soaplib::xml::Node& n);

/// Extract duration from XML, as pointer type
/// @param[in] n XML node
/// @returns Pointer holding the duration information
std::unique_ptr<soaplib::SoapBaseType> DurationPtrFromXml(const soaplib::xml::Node& n);

/// Write duration to XML
/// @param[in] n XML node
/// @param[in] du Duration information
void DurationToXml(soaplib::xml::Node& n, const soaplib::Duration& du);
