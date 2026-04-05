#include "Uuid.hpp"

#include <soaplib/xml/Xml.hpp>
#include <soaplib/ParseHelper.hpp>

namespace soaplib {

Uuid::Uuid()
{
#ifdef WIN32
    UuidCreate(&uuid_);
#else
    uuid_generate(uuid_);
#endif
}

Uuid::Uuid(
    const Uuid& src)
{
#ifdef WIN32
    memcpy(&uuid_, &src.uuid_, sizeof(uuid_));
#else
    uuid_copy(uuid_, src.uuid_);
#endif
}

std::string Uuid::ToString() const
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

Uuid& Uuid::operator=(
    const Uuid& src)
{
#ifdef WIN32
    memcpy(&uuid_, &src.uuid_, sizeof(uuid_));
#else
    uuid_copy(uuid_, src.uuid_);
#endif
    return *this;
}

int Uuid::Compare(
    const Uuid& other) const
{
#ifdef WIN32
    RPC_STATUS status;
    return UuidCompare((UUID*)&uuid_, (UUID*)&other.uuid_, &status);
#else
    return uuid_compare(uuid_, other.uuid_);
#endif
}

void Uuid::ToAnyXml(
    soaplib::xml::Document& doc,
    soaplib::xml::Node& anyNode) const
{
    SetAnyTypeAttribute(doc, anyNode, "guid", "http://schemas.microsoft.com/2003/10/Serialization/", "zer");
    UuidToXml(anyNode, *this);
}

std::string Uuid::GenerateString()
{
    return Uuid().ToString();
}

Uuid Uuid::FromString(
    const std::string& str)
{
    Uuid uuid;

#ifdef WIN32
    UuidFromStringA((RPC_CSTR)str.c_str(), &uuid.uuid_);
#else
    uuid_parse(str.c_str(), uuid.uuid_);
#endif

    return uuid;
}

bool Uuid::ValidateUuidStr(
    const std::string& uuid)
{
    if (uuid.length() != 36)
    {
        return false;
    }

    for (int idx = 0; idx < 36; idx++)
    {
        if (idx == 8 || idx == 13 || idx == 18 || idx == 23)
        {
            if (uuid[idx] != '-')
            {
                return false;
            }
        }
        else
        {
            if (!isxdigit(uuid[idx]))
            {
                return false;
            }
        }
    }

    return true;
}

} // namespace soaplib


void UuidFromXml(
    const soaplib::xml::Node& node,
    soaplib::Uuid& obj)
{
    auto s = node.GetStringVal();
    obj = soaplib::Uuid::FromString(s);
}

soaplib::Uuid UuidFromXml(
    const soaplib::xml::Node& node)
{
    auto obj = soaplib::Uuid{};
    UuidFromXml(node, obj);
    return obj;
}

std::unique_ptr<soaplib::SoapBaseType> UuidPtrFromXml(
    const soaplib::xml::Node& node)
{
    auto obj = std::make_unique<soaplib::Uuid>();
    UuidFromXml(node, *obj.get());
    return obj;
}

void UuidToXml(
    soaplib::xml::Node& node,
    const soaplib::Uuid& value)
{
    node.SetVal(value.ToString());
}
