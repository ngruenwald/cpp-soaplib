#pragma once

#include <string>

namespace soaplib {

class Duration
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
