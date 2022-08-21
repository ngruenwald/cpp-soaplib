#pragma once

#include <memory>
#include <string>

#ifdef WIN32
#include <Rpc.h>
#else // !WIN32
#include <uuid/uuid.h>	// libuuid
#endif // WIN32

#include <soaplib/types/soapBaseType.hpp>

namespace soaplib {

namespace xml { class Node; }

/// Universial Unique Identifier
class uuid
    : public SoapBaseType
{
public:
    /// Default constructor
    uuid();

    /// Copy constructor
    /// @param[in] src The object to copy
    uuid(
        const uuid& src);

    /// Serialize as string
    /// @returns String containing the formatted UUID
    std::string to_string() const;

    /// Assigns another UUID value
    /// @param[in] src The new UUID
    uuid& operator=(
        const uuid& src);

    /// Compare against given value
    /// @param[in] other The UUID to compare against
    /// @returns An integer less than, equal to, or greater than zero.
    ///          Depending if this value is lexigraphically less than,
    ///          equal or greather than the other one.
    int compare(const uuid& other) const;

public:
    /// Generate a new UUID string
    static std::string generate_string();

    /// Parse UUID object from string
    /// @param[in] str String containing an UUID
    /// @returns An UUID object
    static uuid from_string(
        const std::string& str);

    /// Check if the given UUID string is valid
    /// @param[in] uuid String containing an UUID
    /// @returns True if the UUID string is valid, otherwise false
    static bool validate_uuid_str(
        const std::string& uuid);

private:
    void generate();

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

/// Writes the UUID to the output stream.
/// @param[in] os The output stream
/// @param[in] value The UUID object
/// @returns Reference to output stream
inline std::ostream& operator<<(
    std::ostream& os,
    const uuid& value)
{
    os << value.to_string();
    return os;
}

/// Reads the UUID from the input stream.
/// @param[in] is The input stream
/// @param[out] value The UUID object
/// @returns Reference to input stream
inline std::istream& operator>>(
    std::istream& is,
    uuid& value)
{
    std::string uri_str;
    is >> uri_str;
    value = uuid::from_string(uri_str);
    return is;
}

} // namespace soaplib


/// Extracts UUID from XML
/// @param[in] node XML node
/// @param[out] obj UUID object
void uuidFromXml(const soaplib::xml::Node& node, soaplib::uuid& obj);

/// Extracts UUID from XML
/// @param[in] node XML node
/// @returns UUID object
soaplib::uuid uuidFromXml(const soaplib::xml::Node& node);

/// Extracts UUID from XML, as pointer type
/// @param[in] node XML node
/// @returns Pointer holding the UUID object
std::unique_ptr<soaplib::SoapBaseType> uuidPtrFromXml(const soaplib::xml::Node& node);

/// Writes UUID to XML
/// @param[in] node XML node
/// @param[in] value UUID object
void uuidToXml(soaplib::xml::Node& node, const soaplib::uuid& value);
