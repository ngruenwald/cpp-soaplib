#include "typeMap.hpp"

#include <algorithm>
#include <map>

namespace cppgen {

std::map<std::string, std::string> NativeTypes =
{
    // string types
    { "ENTITIES",           "soaplib::String"   },
    { "ENTITY",             "soaplib::String"   },
    { "ID",                 "soaplib::String"   },
    { "IDREF",              "soaplib::String"   },
    { "language",           "soaplib::String"   },
    { "Name",               "soaplib::String"   },
    { "NCName",             "soaplib::String"   },
    { "NMTOKEN",            "soaplib::String"   },
    { "NMTOKENS",           "soaplib::String"   },
    { "normalizedStrings",  "soaplib::String"   },
    { "QName",              "soaplib::String"   },
    { "string",             "soaplib::String"   },
    { "token",              "soaplib::String",  },

    // date types
    { "date",               "soaplib::Date"     },
    { "time",               "soaplib::Time"     },
    { "dateTime",           "soaplib::DateTime" },
    { "duration",           "soaplib::Duration" },
    { "gDay",               "soaplib::Date"     },
    { "gMonth",             "soaplib::Date"     },
    { "gMonthDay",          "soaplib::Date"     },
    { "gYear",              "soaplib::Date"     },
    { "gYearMonth",         "soaplib::Date"     },

    // numeric types
    { "byte",               "soaplib::UInt8"    },
    { "char",               "soaplib::Int8"     },
    { "decimal",            "soaplib::Double"   },
    { "int",                "soaplib::Int32"    },
    { "integer",            "soaplib::Int32"    },
    { "long",               "soaplib::Int64"    },
    { "negativeInteger",    "soaplib::Int32"    },
    { "nonNegativeInteger", "soaplib::Int32"    },
    { "nonPositiveInteger", "soaplib::Int32"    },
    { "positiveInteger",    "soaplib::Int32"    },
    { "short",              "soaplib::Int16"    },
    { "unsignedByte",       "soaplib::UInt8"    },
    { "unsignedShort",      "soaplib::UInt16"   },
    { "unsignedInt",        "soaplib::UInt32"   },
    { "unsignedLong",       "soaplib::UInt64"   },

    // miscellaneous
    { "anyURI",             "soaplib::String"   },
    { "base64Binary",       "soaplib::Base64"   },
    { "boolean",            "soaplib::Bool"     },
    { "double",             "soaplib::Double"   },
    { "float",              "soaplib::Float"    },
    { "hexBinary",          "soaplib::String"   },
    { "float",              "soaplib::Float"    },
    { "NOTATION",           "soaplib::String"   },

    // custom
    { "guid",               "soaplib::uuid"     },
    { "Id",                 "soaplib::Int64"    },
    { "Ref",                "soaplib::Int64"    },
    { "anyType",            "SoapLibAnyType"    },
    { "",                   "soaplib::Void"     },
};


bool IsNativeType(
    const Name& name)
{
    // TODO
    auto it = NativeTypes.find(name.name);

    if (it == NativeTypes.end())
    {
        return false;
    }

    if (it->second == "SoapLibAnyType")
    {
        return false;
    }

    return true;
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
