#include "genCMakeLists.hpp"

#include <ostream>
#include <wsdl.hpp>

#include "options.hpp"
#include "Renderer.hpp"
#include "toJson.hpp"

namespace cppgen {
namespace cmake {

void GenerateCMakeLists(
    std::ostream& stream,
    const Options& options,
    const Definition& definition)
{
    std::string target = definition.name.name;

    if (target.empty())
    {
        target = options.name;
    }

    JsonMapper mapper(options);
    nlohmann::json data;
    data["target"] = target;
    data["options"] = options;
    data["definition"] = mapper.ToJson(definition);

    stream << Renderer::Instance().RenderTemplate("CMakeLists.tpl", data);
}

} // namespace cmake
} // namespace cppgen
