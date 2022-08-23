#include "uuid.hpp"

#include "xml/xml.hpp"
#include "parseHelper.hpp"

namespace soaplib {

uuid::uuid()
{
#ifdef WIN32
    UuidCreate(&uuid_);
#else
    uuid_generate(uuid_);
#endif
}

uuid::uuid(
    const uuid& src)
{
#ifdef WIN32
    memcpy(&uuid_, &src.uuid_, sizeof(uuid_));
#else
    uuid_copy(uuid_, src.uuid_);
#endif
}

std::string uuid::to_string() const
{
    std::string str;

#ifdef WIN32
    RPC_CSTR StringUuid;
    UuidToStringA(&uuid_, &StringUuid);
    str.assign((char*)StringUuid);
    RpcStringFreeA(&StringUuid);
#else // !WIN32
    char s[37];
    uuid_unparse(uuid_, s);
    str.assign(s);
#endif // WIN32

    return str;
}

uuid& uuid::operator=(
    const uuid& src)
{
#ifdef WIN32
    memcpy(&uuid_, &src.uuid_, sizeof(uuid_));
#else
    uuid_copy(uuid_, src.uuid_);
#endif
    return *this;
}

int uuid::compare(
    const uuid& other) const
{
#ifdef WIN32
    RPC_STATUS status;
    return UuidCompare((UUID*)&uuid_, (UUID*)&other.uuid_, &status);
#else
    return uuid_compare(uuid_, other.uuid_);
#endif
}

void uuid::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode) const
{
    setAnyTypeAttribute(doc, anyNode, "guid", "http://schemas.microsoft.com/2003/10/Serialization/", "zer");
    uuidToXml(anyNode, *this);
}

std::string uuid::generate_string()
{
    return uuid().to_string();
}

uuid uuid::from_string(
    const std::string& str)
{
    uuid uuid;

#ifdef WIN32
    UuidFromStringA((RPC_CSTR)str.c_str(), &uuid.uuid_);
#else
    uuid_parse(str.c_str(), uuid.uuid_);
#endif

    return uuid;
}

bool uuid::validate_uuid_str(
    const std::string& uuid)
{
    // UUID FMT: 8-4-4-4-12

    // URI must have 36 characters
    if (uuid.length() != 36)
    {
        return false;
    }

    for (int idx = 0; idx < 36; idx++)
    {
        if (idx == 8 || idx == 13 || idx == 18 || idx == 23)
        {
            // these positions must contain a dash
            if (uuid[idx] != '-')
            {
                return false;
            }
        }
        else
        {
            // these positions must contain a hexadezimal character
            if (!isxdigit(uuid[idx]))
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace soaplib


void uuidFromXml(
    const soaplib::xml::Node& node,
    soaplib::uuid& obj)
{
    auto s = node.GetStringVal();
    obj = soaplib::uuid::from_string(s);
}

soaplib::uuid uuidFromXml(
    const soaplib::xml::Node& node)
{
    auto obj = soaplib::uuid{};
    uuidFromXml(node, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> uuidPtrFromXml(
    const soaplib::xml::Node& node)
{
    auto obj = std::make_unique<soaplib::uuid>();
    uuidFromXml(node, *obj.get());
    return obj;
}

void uuidToXml(
    soaplib::xml::Node& node,
    const soaplib::uuid& value)
{
    node.SetVal(value.to_string());
}
