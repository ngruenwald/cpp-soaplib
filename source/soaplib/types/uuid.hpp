#pragma once

#include <memory>
#include <string>

#ifdef WIN32
#include <Rpc.h>
#else // !WIN32
#include <uuid/uuid.h>	// libuuid
#endif // WIN32

#include <soaplib/types/soapBaseType.hpp>


namespace xml { class Node; }

namespace soaplib {

class uuid
    : public SoapBaseType
{
public:
    uuid(void);
    uuid(const uuid& src);

    std::string to_string(void) const;

#ifdef HAVE_BASE64
    std::string to_base64(void) const;
#endif // HAVE_BASE64

#ifdef HAVE_Z85
    std::string to_z85(void) const;
#endif // HAVE_Z85

    uuid& operator=(const uuid& src);

    int compare(const uuid& other) const;

public:
    static std::string generate_string(void);

#ifdef HAVE_BASE64
    static std::string generate_base64(void);
#endif // HAVE_BASE64

#ifdef HAVE_Z85
    static std::string generate_z85(void);
#endif // HAVE_Z85

    static uuid from_string(
        const std::string& str);
/*
    static uuid from_base64(
        const std::string& b64);
*/
    static bool validate_uuid_str(
        const std::string& uuid);

private:
    void generate(void);

private:
#ifdef WIN32
    UUID uuid_;
#else
    uuid_t uuid_;
#endif

}; // uuid

inline bool operator==(const uuid& a, const uuid& b) { return a.compare(b) == 0; }
inline bool operator!=(const uuid& a, const uuid& b) { return a.compare(b) != 0; }
inline bool operator< (const uuid& a, const uuid& b) { return a.compare(b) <  0; }

// Writes the UUID to the output stream.
inline std::ostream& operator<<(
    std::ostream& os, const uuid& value)
{
	os << value.to_string();
	return os;
}

// Reads the UUID from the input stream.
inline std::istream& operator>>(
    std::istream& is, uuid& value)
{
	std::string uri_str;
	is >> uri_str;
	value = uuid::from_string(uri_str);
	return is;
}

} // namespace soaplib


void uuidFromXml(const xml::Node& node, soaplib::uuid& obj);
soaplib::uuid uuidFromXml(const xml::Node& node);
std::shared_ptr<soaplib::SoapBaseType> uuidPtrFromXml(const xml::Node& node);
void uuidToXml(xml::Node& node, const soaplib::uuid& value);
