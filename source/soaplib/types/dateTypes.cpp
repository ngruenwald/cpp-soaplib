#include "dateTypes.hpp"

#include <iomanip>
#include <sstream>
#include <vector>

#include <libxmlwrp.hpp>


static std::vector<std::string> split(
    const std::string& str,
    char sep,
    int maxSplit = 0)
{
    std::vector<std::string> res;

    int numSplit = 0;
    std::string::size_type beg = 0;

    while (true)
    {
        auto idx = str.find(sep, beg);

        if (idx == std::string::npos)
        {
            res.push_back(str.substr(beg));
            break;
        }

        res.push_back(str.substr(beg, idx - beg));
        beg = idx + 1;
        numSplit += 1;

        if (maxSplit > 0 && numSplit >= maxSplit)
        {
            res.push_back(str.substr(beg));
            break;
        }
    }

    return res;
}

//
// string conversion
//

void from_string(const std::string& s, soaplib::Timezone& tz)
{
    if (s.empty())
    {
        return;
    }

    if (s[0] == 'Z')
    {
        tz.IsUTC = true;
    }
    else
    {
        tz.IsUTC = false;

        auto sep = s.find(':');

        if (sep == std::string::npos)
        {
            tz.Offset = std::stof(s);
        }
        else
        {
            auto hours = std::stof(s.substr(0, sep));
            auto minutes = std::stof(s.substr(sep + 1)) / 60.f;
            if (hours > 0)
            {
                tz.Offset = hours + minutes;
            }
            else
            {
                tz.Offset = hours - minutes;
            }
        }
    }
}

std::string to_string(const soaplib::Timezone& tz)
{
    if (tz.IsUTC)
    {
        return "Z";
    }
    else
    {
        int hours = (int)tz.Offset;
        int minutes = (int)((tz.Offset - hours) * 60);
        std::ostringstream oss;
        oss << (tz.Offset > 0. ? '+' : '-')
            << std::setw(2) << std::setfill('0') << std::abs(hours) << ':'
            << std::setw(2) << std::setfill('0') << std::abs(minutes);
        return oss.str();
    }
}

void to_string(std::string& s, const soaplib::Timezone& tz)
{
    s = to_string(tz);
}


void from_string(const std::string& s, soaplib::Date& d)
{
    const auto parts = split(s, '-', 2);
    d.Year = std::stoi(parts[0]);
    if (parts.size() > 1)
    {
        d.Month = std::stoi(parts[1]);
    }
    if (parts.size() > 2)
    {
        auto sep = parts[2].find_first_of("-+Z");
        if (sep == std::string::npos)
        {
            d.Day = std::stoi(parts[2]);
        }
        else
        {
            d.Day = std::stoi(parts[2].substr(0, sep));
            soaplib::Timezone tz;
            from_string(parts[2].substr(sep), tz);
            d.Timezone = tz;
        }
    }
}

std::string to_string(const soaplib::Date& d)
{
    std::ostringstream oss;
    oss << std::setw(4) << std::setfill('0') << d.Year << '-'
        << std::setw(2) << std::setfill('0') << d.Month << '-'
        << std::setw(2) << std::setfill('0') << d.Day;
    if (d.Timezone)
    {
        oss << to_string(d.Timezone.value());
    }
    return oss.str();
}

void to_string(std::string& s, const soaplib::Date& d)
{
    s = to_string(d);
}


void from_string(const std::string& s, soaplib::Time& t)
{
    const auto parts = split(s, ':');
    t.Hour = std::stoi(parts[0]);
    if (parts.size() > 1)
    {
        t.Minute = std::stoi(parts[1]);
    }
    if (parts.size() > 2)
    {
        auto secfrac =
            [] (const std::string& s, soaplib::Time& t)
            {
                auto sep = s.find('.');
                if (sep == std::string::npos)
                {
                    t.Second = std::stoi(s);
                }
                else
                {
                    t.Second = std::stoi(s.substr(0, sep));
                    t.Fraction = std::stoi(s.substr(sep + 1));
                }
            };

        auto sep = parts[2].find_first_of("-+Z");
        if (sep == std::string::npos)
        {
            secfrac(parts[2], t);
        }
        else
        {
            secfrac(parts[2].substr(0, sep), t);
            soaplib::Timezone tz;
            from_string(parts[2].substr(sep + 1), tz);
            t.Timezone = tz;
        }
    }
}

std::string to_string(const soaplib::Time& t)
{
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << t.Hour << ':'
        << std::setw(2) << std::setfill('0') << t.Minute << ':'
        << std::setw(2) << std::setfill('0') << t.Second;
    if (t.Fraction > 0)
    {
        oss << '.' << t.Fraction;
    }
    if (t.Timezone)
    {
        oss << to_string(t.Timezone.value());
    }
    return oss.str();
}

void to_string(std::string& s, const soaplib::Time& t)
{
    s = to_string(t);
}


void from_string(const std::string& s, soaplib::DateTime& dt)
{
    auto sep = s.find('T');
    if (sep == std::string::npos)
    {
        from_string(s, dt.Date);
    }
    else
    {
        from_string(s.substr(0, sep), dt.Date);
        from_string(s.substr(sep + 1), dt.Time);
        dt.Timezone = dt.Time.Timezone;
    //    dt.Date.Timezone.reset();     // 'reset' will not rest state!
    //    dt.Time.Timezone.reset();
    }
}

std::string to_string(const soaplib::DateTime& dt)
{
    std::ostringstream oss;
    oss << to_string(dt.Date) << 'T' << to_string(dt.Time);
    if (dt.Timezone)
    {
        oss << to_string(dt.Timezone.value());
    }
    return oss.str();
}

void to_string(std::string& s, const soaplib::DateTime& dt)
{
    s = to_string(dt);
}


void from_string(const std::string& s, soaplib::Duration& du)
{
    if (s.empty())
    {
        return;
    }

    if (s[0] != 'P' && (s[0] != '-' || s[1] != 'P'))
    {
        return;
    }

    std::string::size_type beg, end;

    if (s[0] == '-')
    {
        du.IsNegative = true;
        beg = 2;
    }
    else
    {
        du.IsNegative = false;
        beg = 1;
    }

    end = beg;

    bool inTime = false;
    while (end < s.length())
    {
        for (end = beg; end < s.length() && std::isdigit(s[end]); ++end);
        auto val = beg != end ? std::stoi(s.substr(beg, end - beg)) : 0;

        switch (s[end])
        {
            case 'Y': du.Years = val; break;
            case 'M': inTime ? du.Minutes = val : du.Months = val; break;
            case 'D': du.Days = val; break;
            case 'T': inTime = true; break;
            case 'H': du.Hours = val; break;
            case 'S': du.Seconds = val; break;
            default: break;
        }

        beg = end + 1;
    }
}

std::string to_string(const soaplib::Duration& du)
{
    std::ostringstream oss;
    if (du.IsNegative)
    {
        oss << '-';
    }
    oss << 'P';
    if (du.Years > 0)  oss << du.Years  << 'Y';
    if (du.Months > 0) oss << du.Months << 'M';
    if (du.Days > 0)   oss << du.Days   << 'D';

    if (du.Hours > 0 || du.Minutes > 0 || du.Seconds > 0)
    {
        oss << 'T';
        if (du.Hours > 0)   oss << du.Hours   << 'H';
        if (du.Minutes > 0) oss << du.Minutes << 'M';
        if (du.Seconds > 0) oss << du.Seconds << 'S';
    }
    return oss.str();
}

void to_string(std::string& s, const soaplib::Duration& du)
{
    s = to_string(du);
}


//
// xml conversion
//

void DateFromXml(const xml::Node& n, soaplib::Date& d)
{
    from_string(n.GetStringVal(), d);
}

soaplib::Date DateFromXml(const xml::Node& n)
{
    soaplib::Date d;
    DateFromXml(n, d);
    return d;
}

std::shared_ptr<soaplib::SoapBaseType> DatePtrFromXml(const xml::Node& n)
{
    auto ptr = std::make_shared<soaplib::Date>();
    DateFromXml(n, *ptr.get());
    return ptr;
}

void DateToXml(xml::Node& n, const soaplib::Date& d)
{
    n.SetVal(to_string(d));
}

void TimeFromXml(const xml::Node& n, soaplib::Time& t)
{
    from_string(n.GetStringVal(), t);
}

soaplib::Time TimeFromXml(const xml::Node& n)
{
    soaplib::Time t;
    TimeFromXml(n, t);
    return t;
}

std::shared_ptr<soaplib::SoapBaseType> TimePtrFromXml(const xml::Node& n)
{
    auto ptr = std::make_shared<soaplib::Time>();
    TimeFromXml(n, *ptr.get());
    return ptr;
}

void TimeToXml(xml::Node& n, const soaplib::Time& t)
{
    n.SetVal(to_string(t));
}

void DateTimeFromXml(const xml::Node& n, soaplib::DateTime& dt)
{
    from_string(n.GetStringVal(), dt);
}

soaplib::DateTime DateTimeFromXml(const xml::Node& n)
{
    soaplib::DateTime dt;
    DateTimeFromXml(n, dt);
    return dt;
}

std::shared_ptr<soaplib::SoapBaseType> DateTimePtrFromXml(const xml::Node& n)
{
    auto ptr = std::make_shared<soaplib::DateTime>();
    DateTimeFromXml(n, *ptr.get());
    return ptr;
}

void DateTimeToXml(xml::Node& n, const soaplib::DateTime& dt)
{
    n.SetVal(to_string(dt));
}

void DurationFromXml(const xml::Node& n, soaplib::Duration& du)
{
    from_string(n.GetStringVal(), du);
}

soaplib::Duration DurationFromXml(const xml::Node& n)
{
    soaplib::Duration du;
    DurationFromXml(n, du);
    return du;
}

std::shared_ptr<soaplib::Duration> DurationPtrFromXml(const xml::Node& n)
{
    auto ptr = std::make_shared<soaplib::Duration>();
    DurationFromXml(n, *ptr.get());
    return ptr;
}

void DurationToXml(xml::Node& n, const soaplib::Duration& du)
{
    n.SetVal(to_string(du));
}
