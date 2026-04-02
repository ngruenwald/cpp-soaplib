#include "toJson.hpp"
#include "typeMap.hpp"
#include "utils.hpp"
#include "genServiceHelper.hpp"
#include <algorithm>

using namespace cppgen;

namespace cppgen {

JsonMapper::JsonMapper(const Options& options)
    : options_(options)
{
}

bool JsonMapper::IsPointerType(const Name& name)
{
    for (const auto& pt : options_.pointerTypes)
    {
        if (name.name == pt)
        {
            return true;
        }
    }
    return false;
}

bool JsonMapper::IsInnerType(const Name& name, const std::vector<TypePtr>& innerTypes)
{
    for (const auto& innerType : innerTypes)
    {
        if (name.name == innerType->name.name)
        {
            return true;
        }
    }
    return false;
}

nlohmann::json JsonMapper::ToJson(const Name& n)
{
    return nlohmann::json{
        {"name", n.name},
        {"prefix", n.nsPrefix},
        {"href", n.nsHref}
    };
}

nlohmann::json JsonMapper::ToJson(const Type& t)
{
    nlohmann::json j;
    j["name"] = ResolveType(t.name, options_, true);
    j["wsdl_name"] = t.name.name;
    j["xmlns"] = ToJson(t.name);
    j["kind_enum"] = (int)t.kind;
    j["isNativeType"] = IsNativeType(t.name);
    j["resolved_name"] = ResolveType(t.name, options_, true);
    j["full_resolved_name"] = ResolveType(t.name, options_, false);
    j["namespaces"] = options_.namespaces;

    switch (t.kind)
    {
        case Type::Basic:
        {
            const auto& bt = static_cast<const BasicType&>(t);
            j["kind"] = "basic";
            if (bt.base.has_value()) {
                j["base"]["name"] = ResolveType(bt.base.value(), options_, true);
                j["base"]["xmlns"] = ToJson(bt.base.value());
                j["base"]["resolved_name"] = ResolveType(bt.base.value(), options_, true);
            }
            break;
        }
        case Type::Extended:
        {
            const auto& et = static_cast<const ExtendedType&>(t);
            j["kind"] = "struct";
            if (et.base.has_value()) {
                j["base"]["name"] = ResolveType(et.base.value(), options_, true);
                j["base"]["xmlns"] = ToJson(et.base.value());
                j["base"]["resolved_name"] = ResolveType(et.base.value(), options_, true);
            }
            j["struct"]["parameters"] = nlohmann::json::array();
            for (const auto& p : et.parameters) {
                j["struct"]["parameters"].push_back(ToJson(p, et));
            }
            j["struct"]["innerTypes"] = nlohmann::json::array();
            for (const auto& it : et.innerTypes) {
                j["struct"]["innerTypes"].push_back(ToJson(*it));
            }
            break;
        }
        case Type::Enum:
        {
            const auto& et = static_cast<const EnumType&>(t);
            j["kind"] = "enum";
            j["enum"]["enumerations"] = nlohmann::json::array();
            for (const auto& e : et.enumerations) {
                nlohmann::json ej;
                ej["text"] = e.text;
                if (e.value.has_value()) ej["value"] = e.value.value();
                j["enum"]["enumerations"].push_back(ej);
            }
            break;
        }
    }
    return j;
}

nlohmann::json JsonMapper::ToJson(const Parameter& p, const ExtendedType& et)
{
    nlohmann::json j = {
        {"name", FormatParameterName(p.name.name)},
        {"wsdl_name", p.name.name},
        {"xmlns", ToJson(p.name)},
        {"type", ResolveType(p.type, options_, true)},
        {"wsdl_type", ToJson(p.type)},
        {"kind_enum", (int)p.kind},
        {"resolved_type", ResolveType(p.type, options_, true)},
        {"full_resolved_type", ResolveType(p.type, options_, false)},
        {"isNativeType", IsNativeType(p.type)},
        {"isInnerType", IsInnerType(p.type, et.innerTypes)},
        {"cpp_name", FormatParameterName(p.name.name)}
    };

    switch (p.kind)
    {
        case Parameter::Mandatory: j["kind"] = "mandatory"; break;
        case Parameter::Pointer:   j["kind"] = "pointer";   break;
        case Parameter::Optional:  j["kind"] = "optional";  break;
        case Parameter::Multiple:  j["kind"] = "multiple";  break;
    }

    bool usePointer = IsPointerType(p.name);
    j["isPointerType"] = (p.kind == Parameter::Pointer) || (p.kind == Parameter::Optional && usePointer) || (p.kind == Parameter::Multiple && usePointer);
    j["isOptionalType"] = (p.kind == Parameter::Optional);
    j["isMultipleType"] = (p.kind == Parameter::Multiple);
    j["needsForwardDeclaration"] = options_.cyclicTypes.count(p.type.name) > 0 && !j["isInnerType"].get<bool>();

    if (IsInnerType(p.name, et.innerTypes))
    {
        j["prefix"] = et.name.name + "::";
        j["suffix"] = options_.innerTypeSuffix;
    }
    else
    {
        j["prefix"] = "";
        j["suffix"] = "";
    }

    return j;
}

nlohmann::json JsonMapper::ToJson(const Message& m)
{
    nlohmann::json j;
    j["name"] = m.name.name;
    j["xmlns"] = ToJson(m.name);
    j["parts"] = nlohmann::json::array();
    for (const auto& p : m.parts) {
        j["parts"].push_back({
            {"name", FormatParameterName(p.name.name)},
            {"wsdl_name", p.name.name},
            {"xmlns", ToJson(p.name)},
            {"element", p.element.name},
            {"xmlns_element", ToJson(p.element)},
            {"resolved_element", ResolveType(p.element, options_, true)}
        });
    }
    return j;
}

nlohmann::json JsonMapper::ToJson(const Port& p)
{
    return nlohmann::json{
        {"name", p.name.name},
        {"xmlns", ToJson(p.name)},
        {"binding", p.binding.name},
        {"xmlns_binding", ToJson(p.binding)},
        {"address", {{"location", p.address.location}}},
        {"endpointReference", {{"address", p.endpointReference.address}}}
    };
}

nlohmann::json JsonMapper::ToJson(const Service& s, const Definition& d)
{
    nlohmann::json ports = nlohmann::json::array();
    nlohmann::json operations = nlohmann::json::array();

    for (const auto& port : s.ports) {
        if (!isPortAllowed(options_, port)) continue;
        
        auto pj = ToJson(port);
        ports.push_back(pj);

        const auto binding = service::getBinding(port.binding, d);
        if (!binding) continue;

        const auto portType = service::getPortType(binding->type, d);
        if (!portType) continue;

        for (const auto& op : portType->operations) {
            operations.push_back(ToJson(op, d));
        }
    }

    return nlohmann::json{
        {"name", safe_name(s.name.name)},
        {"wsdl_name", s.name.name},
        {"xmlns", ToJson(s.name)},
        {"ports", ports},
        {"operations", operations},
        {"safe_name", safe_name(s.name.name)},
        {"version", (d.version == soaplib::SoapVersion::Soap11) ? "1.1" : "1.2"}
    };
}

nlohmann::json JsonMapper::ToJson(const Input& i)
{
    return nlohmann::json{
        {"action", i.action.name},
        {"xmlns_action", ToJson(i.action)},
        {"message", i.message.name},
        {"xmlns_message", ToJson(i.message)}
    };
}

nlohmann::json JsonMapper::ToJson(const Output& o)
{
    return nlohmann::json{
        {"action", o.action.name},
        {"xmlns_action", ToJson(o.action)},
        {"message", o.message.name},
        {"xmlns_message", ToJson(o.message)}
    };
}

nlohmann::json JsonMapper::ToJson(const Operation& o, const Definition& d)
{
    nlohmann::json j = {
        {"name", o.name.name},
        {"wsdl_name", o.name.name},
        {"xmlns", ToJson(o.name)},
        {"input", ToJson(o.input)},
        {"output", ToJson(o.output)}
    };

    if (!o.input.message.name.empty()) {
        auto inputTypes = service::getMessagePartNames(o.input.message, d);
        if (!inputTypes.empty()) {
            j["input_type"] = inputTypes[0].name;
            j["input_resolved_type"] = ResolveType(inputTypes[0], options_, true);
            j["input_full_resolved_type"] = ResolveType(inputTypes[0], options_, false);
            j["input_is_native"] = IsNativeType(inputTypes[0]);
        }
    }

    if (!o.output.message.name.empty()) {
        auto outputTypes = service::getMessagePartNames(o.output.message, d);
        if (!outputTypes.empty()) {
            j["output_type"] = outputTypes[0].name;
            j["output_resolved_type"] = ResolveType(outputTypes[0], options_, true);
            j["output_full_resolved_type"] = ResolveType(outputTypes[0], options_, false);
            j["output_is_native"] = IsNativeType(outputTypes[0]);
        } else {
            j["output_resolved_type"] = "void";
        }
    } else {
        j["output_resolved_type"] = "void";
    }

    return j;
}

nlohmann::json JsonMapper::ToJson(const PortType& p, const Definition& d)
{
    nlohmann::json ops = nlohmann::json::array();
    for (const auto& o : p.operations) ops.push_back(ToJson(o, d));

    return nlohmann::json{
        {"name", p.name.name},
        {"xmlns", ToJson(p.name)},
        {"operations", ops}
    };
}

nlohmann::json JsonMapper::ToJson(const Binding& b, const Definition& d)
{
    nlohmann::json ops = nlohmann::json::array();
    for (const auto& o : b.operations) ops.push_back(ToJson(o, d));

    return nlohmann::json{
        {"name", b.name.name},
        {"xmlns", ToJson(b.name)},
        {"type", b.type.name},
        {"xmlns_type", ToJson(b.type)},
        {"operations", ops}
    };
}

nlohmann::json JsonMapper::ToJson(const Definition& d)
{
    nlohmann::json j;
    j["name"] = d.name.name;
    j["xmlns"] = ToJson(d.name);
    j["version"] = (d.version == soaplib::SoapVersion::Soap11) ? "1.1" : "1.2";
    j["types"] = nlohmann::json::array();
    for (const auto& t : d.types) {
        if (t) j["types"].push_back(ToJson(*t));
    }
    
    j["messages"] = nlohmann::json::array();
    for (const auto& m : d.messages) j["messages"].push_back(ToJson(m));

    j["portTypes"] = nlohmann::json::array();
    for (const auto& p : d.portTypes) j["portTypes"].push_back(ToJson(p, d));

    j["bindings"] = nlohmann::json::array();
    for (const auto& b : d.bindings) j["bindings"].push_back(ToJson(b, d));

    j["services"] = nlohmann::json::array();
    for (const auto& s : d.services) j["services"].push_back(ToJson(s, d));

    return j;
}

nlohmann::json JsonMapper::ToJson(const Options& o)
{
    return nlohmann::json{
        {"name", o.name},
        {"outputPath", o.outputPath},
        {"appendNamespacesToPath", o.appendNamespacesToPath},
        {"typesSubfolder", o.typesSubfolder},
        {"namespaces", o.namespaces},
        {"cmakeNamespace", o.cmakeNamespace},
        {"cmakeExport", o.cmakeExport},
        {"innerTypeSuffix", o.innerTypeSuffix},
        {"generateClient", o.generateClient},
        {"generateServer", o.generateServer},
        {"abortOnUnknownType", o.abortOnUnknownType},
        {"writeTimestamp", o.writeTimestamp}
    };
}

void to_json(nlohmann::json& j, const Options& o)
{
    j = nlohmann::json{
        {"name", o.name},
        {"outputPath", o.outputPath},
        {"appendNamespacesToPath", o.appendNamespacesToPath},
        {"typesSubfolder", o.typesSubfolder},
        {"namespaces", o.namespaces},
        {"cmakeNamespace", o.cmakeNamespace},
        {"cmakeExport", o.cmakeExport},
        {"innerTypeSuffix", o.innerTypeSuffix},
        {"generateClient", o.generateClient},
        {"generateServer", o.generateServer},
        {"abortOnUnknownType", o.abortOnUnknownType},
        {"writeTimestamp", o.writeTimestamp}
    };
}

} // namespace cppgen

// Dummy implementations for global to_json to satisfy compiler if needed by other things,
// but we prefer JsonMapper.
void to_json(nlohmann::json& j, const Name& n) { j = JsonMapper(Options()).ToJson(n); }
void to_json(nlohmann::json& j, const Type& t) { j = JsonMapper(Options()).ToJson(t); }
//void to_json(nlohmann::json& j, const Parameter& p) { j = JsonMapper(Options()).ToJson(p); }
void to_json(nlohmann::json& j, const Message& m) { j = JsonMapper(Options()).ToJson(m); }
void to_json(nlohmann::json& j, const Port& p) { j = JsonMapper(Options()).ToJson(p); }
void to_json(nlohmann::json& j, const Service& s) { j = JsonMapper(Options()).ToJson(s, Definition()); }
void to_json(nlohmann::json& j, const Input& i) { j = JsonMapper(Options()).ToJson(i); }
void to_json(nlohmann::json& j, const Output& o) { j = nlohmann::json{{"action", o.action.name}, {"message", o.message.name}}; }
void to_json(nlohmann::json& j, const Operation& o) { j = JsonMapper(Options()).ToJson(o, Definition()); }
void to_json(nlohmann::json& j, const PortType& p) { j = JsonMapper(Options()).ToJson(p, Definition()); }
void to_json(nlohmann::json& j, const Binding& b) { j = JsonMapper(Options()).ToJson(b, Definition()); }
void to_json(nlohmann::json& j, const Definition& d) { j = JsonMapper(Options()).ToJson(d); }
