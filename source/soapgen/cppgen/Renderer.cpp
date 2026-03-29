#include "Renderer.hpp"

#include <iostream>
#include <cassert>

#include <inja/inja.hpp>

namespace cppgen {

Renderer* Renderer::Instance_ = nullptr;

Renderer::Renderer(
    const std::string& inputPath,
    const std::string& outputPath)
    : env_{std::make_unique<inja::Environment>(inputPath, outputPath)}
{
    assert(Instance_ == nullptr);
    Instance_ = this;

    auto fmacro =
        [this](inja::Arguments& args) -> nlohmann::json
        {
            auto file = args.at(0)->get<std::string>();
            auto data = args.at(1)->get<nlohmann::json>();
            auto tmpl = env_->parse_template(file);
            return env_->render(tmpl, data);
        };

    auto indent =
        [](inja::Arguments& args) -> nlohmann::json
        {
            auto indent_size = args.at(0)->get<int>();
            auto input = args.at(1)->get<std::string>();

            std::string indent_str(indent_size, ' ');

            std::ostringstream oss;
            std::istringstream iss{input};

            for (std::string line; std::getline(iss, line); )
            {
                if (!line.empty()) {
                    oss << indent_str << line << '\n';
                } else {
                    oss << '\n';
                }
            }

            return oss.str();
        };

    auto concat =
        [](inja::Arguments& args) -> nlohmann::json
        {
            auto parts = args.at(0)->get<std::vector<std::string>>();
            auto separator = args.at(1)->get<std::string>();
            bool sae = args.size() > 2 ? args.at(2)->get<bool>() : false;

            std::ostringstream oss;

            if (parts.size() > 0)
            {
                oss << parts[0];

                for (std::size_t n = 1; n < parts.size(); n++)
                {
                    oss << separator << parts[n];
                }
            }

            if (sae)
            {
                oss << separator;
            }

            return oss.str();
        };

    env_->add_callback("fmacro", fmacro);
    env_->add_callback("indent", indent);
    env_->add_callback("concat", concat);

    env_->set_trim_blocks(true);
    env_->set_lstrip_blocks(true);
}

Renderer::~Renderer()
{
    assert(Instance_ == this);
    Instance_ = nullptr;
}

Renderer& Renderer::Instance()
{
    assert(Instance_);
    return *Instance_;
}

std::string Renderer::Render(
    const std::string& input,
    const nlohmann::json& data)
{
    auto tmpl = env_->parse(input);
    return env_->render(tmpl, data);
}

std::string Renderer::RenderTemplate(
    const std::string& fileName,
    const nlohmann::json& data)
{
    auto tmpl = env_->parse_file(fileName);
    return env_->render(tmpl, data);
}

} // namespace cppgen
