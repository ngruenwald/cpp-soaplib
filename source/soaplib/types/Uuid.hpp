#pragma once

#include <memory>
#include <string>
#include <iostream>

#ifdef WIN32
#include <Rpc.h>
#else // !WIN32
#include <uuid/uuid.h>
#endif // WIN32

#include <soaplib/types/SoapBaseType.hpp>

namespace soaplib {

namespace xml { class Node; }

/// Universal Unique Identifier
class Uuid
    : public SoapBaseType
{
public:
    Uuid();
    Uuid(const Uuid& src);

    std::string ToString() const;

    Uuid& operator=(const Uuid& src);

    int Compare(const Uuid& other) const;

    void ToAnyXml(
        soaplib::xml::Document& doc,
        soaplib::xml::Node& anyNode) const override;

public:
    static std::string GenerateString();
    static Uuid FromString(const std::string& str);
    static bool ValidateUuidStr(const std::string& uuid);

private:
#ifdef WIN32
    UUID uuid_;
#else
    uuid_t uuid_;
#endif
};

inline bool operator==(const Uuid& a, const Uuid& b) { return a.Compare(b) == 0; }
inline bool operator!=(const Uuid& a, const Uuid& b) { return a.Compare(b) != 0; }
inline bool operator< (const Uuid& a, const Uuid& b) { return a.Compare(b) <  0; }

inline std::ostream& operator<<(std::ostream& os, const Uuid& value) {
    os << value.ToString();
    return os;
}

inline std::istream& operator>>(std::istream& is, Uuid& value) {
    std::string uri_str;
    is >> uri_str;
    value = Uuid::FromString(uri_str);
    return is;
}

} // namespace soaplib

void UuidFromXml(const soaplib::xml::Node& node, soaplib::Uuid& obj);
soaplib::Uuid UuidFromXml(const soaplib::xml::Node& node);
std::unique_ptr<soaplib::SoapBaseType> UuidPtrFromXml(const soaplib::xml::Node& node);
void UuidToXml(soaplib::xml::Node& node, const soaplib::Uuid& value);
