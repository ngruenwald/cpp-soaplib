#pragma once

#include <nlohmann/json.hpp>
#include "../wsdl.hpp"
#include "options.hpp"

// Forward declarations in global namespace for ADL
void to_json(nlohmann::json& j, const Name& n);
void to_json(nlohmann::json& j, const Type& t);
void to_json(nlohmann::json& j, const Parameter& p);
void to_json(nlohmann::json& j, const Message& m);
void to_json(nlohmann::json& j, const Port& p);
void to_json(nlohmann::json& j, const Service& s);
void to_json(nlohmann::json& j, const Input& i);
void to_json(nlohmann::json& j, const Output& o);
void to_json(nlohmann::json& j, const Operation& o);
void to_json(nlohmann::json& j, const PortType& p);
void to_json(nlohmann::json& j, const Binding& b);
void to_json(nlohmann::json& j, const Definition& d);

namespace cppgen {

class JsonMapper
{
public:
    JsonMapper(const Options& options);

    nlohmann::json ToJson(const Name& n);
    nlohmann::json ToJson(const Type& t);
    nlohmann::json ToJson(const Parameter& p, const ExtendedType& et);
    nlohmann::json ToJson(const Message& m);
    nlohmann::json ToJson(const Port& p);
    nlohmann::json ToJson(const Service& s, const Definition& d);
    nlohmann::json ToJson(const Input& i);
    nlohmann::json ToJson(const Output& o);
    nlohmann::json ToJson(const Operation& o, const Definition& d);
    nlohmann::json ToJson(const PortType& p, const Definition& d);
    nlohmann::json ToJson(const Binding& b, const Definition& d);
    nlohmann::json ToJson(const Definition& d);
    nlohmann::json ToJson(const Options& o);

private:
    bool IsPointerType(const Name& name);
    bool IsInnerType(const Name& name, const std::vector<TypePtr>& innerTypes);

private:
    const Options& options_;
};

void to_json(nlohmann::json& j, const Options& o);

} // namespace cppgen

