#include "genTypeImpl.hpp"

#include "typeMap.hpp"
#include "utils.hpp"
#include "Renderer.hpp"
#include "toJson.hpp"

#include <iostream>

namespace cppgen {
namespace type {

void GenerateImplementation(
    std::ostream& stream,
    const Type& type,
    const Options& options,
    const Definition& definition)
{
    JsonMapper mapper(options);
    nlohmann::json data;
    data["type"] = mapper.ToJson(type);
    data["options"] = options;
    data["meta"]["version"] = "0.2.0"; // TODO: get from somewhere
    data["meta"]["timestamp"] = now();

    stream << Renderer::Instance().RenderTemplate("TypeImpl.tpl", data);
}

} // namespace type
} // namespace cppgen
