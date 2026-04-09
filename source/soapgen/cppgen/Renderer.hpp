#pragma once

#include <memory>
#include <string>

#include <nlohmann/json_fwd.hpp>

namespace inja { class Environment; }

namespace cppgen {

class Renderer
{
public:
    Renderer(
        const std::string& inputPath,
        const std::string& outputPath);
    ~Renderer();

    static Renderer& Instance();

public:
    std::string Render(
        const std::string& input,
        const nlohmann::json& data);

    std::string RenderTemplate(
        const std::string& fileName,
        const nlohmann::json& data);

private:
    std::string GetEmbeddedTemplate(const std::string& fileName) const;

public:
    std::unique_ptr<inja::Environment> env_;
    std::string templatePath_;

private:
    static Renderer* Instance_;
};

} // namespace cppgen
