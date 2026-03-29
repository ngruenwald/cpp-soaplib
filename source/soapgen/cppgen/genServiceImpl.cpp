#include "genServiceImpl.hpp"

#include "typeMap.hpp"
#include "utils.hpp"
#include "Renderer.hpp"
#include "toJson.hpp"

#include <iostream>

namespace cppgen {
namespace service {

void GenerateImplementation(
    std::ostream& stream,
    const Service& service,
    const Options& options,
    const Definition& definition)
{
    JsonMapper mapper(options);
    nlohmann::json data;
    data["service"] = mapper.ToJson(service, definition);
    data["options"] = options;
    data["meta"]["version"] = "0.2.0";
    data["meta"]["timestamp"] = now();

    stream << Renderer::Instance().RenderTemplate("ServiceImpl.tpl", data);
}

} // namespace service
} // namespace cppgen
