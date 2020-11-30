#include "duration.hpp"

#include <libxmlwrp.hpp>


namespace soaplib {


std::string Duration::ToString(const Duration& t)
{
    std::ostringstream oss;
    oss << 'P';
    if (t.Years() > 0) oss << t.Years() << 'Y';
    if (t.Months() > 0) oss << t.Months() << 'M';
    if (t.Days() > 0) oss << t.Days() << 'D';

    if (t.Hours() > 0 || t.Minutes() > 0 || t.Seconds() > 0)
    {
        oss << 'T';
        if (t.Hours() > 0) oss << t.Hours() << 'H';
        if (t.Minutes() > 0) oss << t.Minutes() << 'M';
        if (t.Seconds() > 0) oss << t.Seconds() << 'S';
    }

    return oss.str();
}

Duration Duration::FromString(const std::string& s)
{
    auto d = Duration{};

    if (!s.empty() && s[0] == 'P')
    {
        bool inTime = false;
        std::string::size_type beg = 1, end = 1;
        while (end < s.length())
        {
            for (end = beg; end < s.length() && std::isdigit(s[end]); ++end);
            auto val = beg != end ? std::stoi(s.substr(beg, end - beg)) : 0;

            switch (s[end])
            {
                case 'Y': d.years_ = val; break;
                case 'M': inTime ? d.minutes_ = val : d.months_ = val; break;
                case 'D': d.days_ = val; break;
                case 'T': inTime = true; break;
                case 'H': d.hours_ = val; break;
                case 'S': d.seconds_ = val; break;
                default: break;
            }

            beg = end + 1;
        }
    }

	return d;
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
