#include "genAnyTypeHeader.hpp"

#include "typeMap.hpp"
#include "utils.hpp"
#include "Renderer.hpp"
#include "toJson.hpp"

#include <iostream>

namespace cppgen {
namespace type {

void GenerateAnyTypeHeader(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    JsonMapper mapper(options);
    nlohmann::json data;
    data["type"]["name"] = "SoapLibAnyType";
    data["options"] = options;
    data["definition"] = mapper.ToJson(definition);
    data["meta"]["version"] = "0.2.0";
    data["meta"]["timestamp"] = now();

    stream << Renderer::Instance().RenderTemplate("AnyTypeHeader.tpl", data);
}

} // namespace type
} // namespace cppgen
