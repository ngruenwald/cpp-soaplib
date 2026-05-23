#include "Renderer.hpp"
#include "resources.h"

#include <iostream>
#include <cassert>
#include <filesystem>
#include <map>

#include <inja/inja.hpp>

namespace cppgen {

Renderer* Renderer::Instance_ = nullptr;
Renderer::Renderer(
    const std::string& inputPath,
    const std::string& outputPath)
    : templatePath_(inputPath)
{
    std::string base = inputPath;
    if (!base.empty() && base.back() != '/' && base.back() != '\\') {
        base += "/";
    }
    env_ = std::make_unique<inja::Environment>(base, outputPath);

    assert(Instance_ == nullptr);

    Instance_ = this;

    auto fmacro =
        [this](inja::Arguments& args) -> nlohmann::json
        {
            auto file = args.at(0)->get<std::string>();
            auto data = args.at(1)->get<nlohmann::json>();

            if (templatePath_.empty()) {
                auto embedded = GetEmbeddedTemplate(file);
                if (!embedded.empty()) {
                    return env_->render(embedded, data);
                }
            }

            auto tmpl = env_->load_file(file);
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

            if (sae && !parts.empty())
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
    if (templatePath_.empty()) {
        auto embedded = GetEmbeddedTemplate(fileName);
        if (!embedded.empty()) {
            return env_->render(embedded, data);
        }
    }
    return env_->render_file(fileName, data);
}

std::string Renderer::GetEmbeddedTemplate(const std::string& fileName) const
{
    static const std::map<std::string, std::pair<const unsigned char*, unsigned long>> embeddedTemplates = {
        {"AnyTypeHeader.tpl", {AnyTypeHeader_tpl_data, AnyTypeHeader_tpl_size}},
        {"AnyTypeImpl.tpl", {AnyTypeImpl_tpl_data, AnyTypeImpl_tpl_size}},
        {"CMakeLists.tpl", {CMakeLists_tpl_data, CMakeLists_tpl_size}},
        {"ServerHeader.tpl", {ServerHeader_tpl_data, ServerHeader_tpl_size}},
        {"ServerImpl.tpl", {ServerImpl_tpl_data, ServerImpl_tpl_size}},
        {"ServiceHeader.tpl", {ServiceHeader_tpl_data, ServiceHeader_tpl_size}},
        {"ServiceImpl.tpl", {ServiceImpl_tpl_data, ServiceImpl_tpl_size}},
        {"TypeHeader_ForwardDeclarations.tpl", {TypeHeader_ForwardDeclarations_tpl_data, TypeHeader_ForwardDeclarations_tpl_size}},
        {"TypeHeader_Includes.tpl", {TypeHeader_Includes_tpl_data, TypeHeader_Includes_tpl_size}},
        {"TypeHeader_TypeDeclaration.tpl", {TypeHeader_TypeDeclaration_tpl_data, TypeHeader_TypeDeclaration_tpl_size}},
        {"TypeHeader.tpl", {TypeHeader_tpl_data, TypeHeader_tpl_size}},
        {"TypeImpl_Includes.tpl", {TypeImpl_Includes_tpl_data, TypeImpl_Includes_tpl_size}},
        {"TypeImpl_Serializer.tpl", {TypeImpl_Serializer_tpl_data, TypeImpl_Serializer_tpl_size}},
        {"TypeImpl.tpl", {TypeImpl_tpl_data, TypeImpl_tpl_size}},
    };

    auto it = embeddedTemplates.find(fileName);
    if (it != embeddedTemplates.end()) {
        return std::string(reinterpret_cast<const char*>(it->second.first), it->second.second);
    }
    return {};
}

} // namespace cppgen
