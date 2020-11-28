#include "typeMap.hpp"

#include <algorithm>
#include <map>

namespace cppgen {

std::map<std::string, std::string> NativeTypes =
{
    { "byte",           "std::uint8_t"       },
    { "boolean",        "bool"               },
    { "string",         "std::string"        },
    { "anyURI",         "std::string"        },
    { "QName",          "std::string"        },
    { "duration",       "soaplib::Duration"  },
    { "dateTime",       "soaplib::Timestamp" },
    { "guid",           "soaplib::uuid"      },
    { "base64Binary",   "soaplib::Base64"    },
    { "decimal",        "double"             },
    { "double",         "double"             },
    { "float",          "float"              },
    { "char",           "std::int"           },
    { "short",          "std::int16_t"       },
    { "int",            "std::int32_t"       },
    { "long",           "std::int64_t"       },
    { "unsignedByte",   "std::uint8_t"       },
    { "unsignedShort",  "std::uint16_t"      },
    { "unsignedInt",    "std::uint32_t"      },
    { "unsignedLong",   "std::uint64_t"      },
    { "Id",             "long"               },
    { "Ref",            "long"               },
    { "anyType",        "SoapLibAnyType"     },
    { "",               "void"               },
};


bool IsNativeType(
    const Name& name)
{
    // TODO
    auto it = NativeTypes.find(name.name);

    if (it->second == "SoapLibAnyType")
    {
        return false;
    }

    return it != NativeTypes.end();
}

std::string ResolveType(
    const Name& name,
    bool stripNamespace)
{
    auto it = NativeTypes.find(name.name);
    auto tp = it == NativeTypes.end() ? name.name : it->second;

    std::replace(tp.begin(), tp.end(), '.', '_');

#ifdef REPLACE
    if (stripNamespace)
    {
        std::replace(tp.begin(), tp.end(), ':', '_');
    }

    return tp;
#else
    if (!stripNamespace)
    {
        return tp;
    }

    auto idx = tp.rfind("::");

    if (idx == std::string::npos)
    {
        return tp;
    }

    return tp.substr(idx + 2);
#endif
}

std::string FormatParameterName(
    const std::string& name)
{
    std::string cppName(name);
    cppName[0] = std::toupper(cppName[0]);
    return cppName;
}

} // namespace cppgen
