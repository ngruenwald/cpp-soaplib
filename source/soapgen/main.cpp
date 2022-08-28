#include <iostream>

#include "wsdl.hpp"
#include "cppgen/cppgen.hpp"

#include "soaplib/xml/xml.hpp"

#include <argparse/argparse.hpp>

struct Config
{
    std::string wsdlFile;
    cppgen::Options cpp;
};

std::unique_ptr<Config> LoadConfig(
    const std::string& configFile)
{
    try
    {
        auto doc = soaplib::xml::Document::ParseFile(configFile.c_str());

        auto config = std::make_unique<Config>();

        XML_MANDATORY(config->wsdlFile = doc.GetNode("/config/wsdl").GetStringProp("path"));

        XML_OPTIONAL(config->cpp.name = doc.GetNode("/config").GetStringProp("name"));
        XML_OPTIONAL(config->cpp.outputPath = doc.GetNode("/config/cpp/output").GetStringProp("path"));
        XML_OPTIONAL(config->cpp.appendNamespacesToPath = doc.GetNode("/config/cpp/output").GetBoolProp("append-ns"));
        XML_OPTIONAL(config->cpp.typesSubfolder = doc.GetNode("/config/cpp/output").GetStringProp("types-subfolder"));

        try
        {
            auto nsNodes = doc.GetNodes("/config/cpp/namespaces/namespace");
            for (const auto& nsNode : nsNodes)
            {
                config->cpp.namespaces.push_back(nsNode.GetStringVal());
            }
        }
        catch(const std::exception& e)
        {
        }

        try
        {
            auto ptrNodes = doc.GetNodes("/config/cpp/pointer-types/param");
            for (const auto& ptrNode : ptrNodes)
            {
                config->cpp.pointerTypes.push_back(ptrNode.GetStringVal());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }


        XML_OPTIONAL(config->cpp.cmakeNamespace = doc.GetNode("/config/cpp/cmake").GetStringProp("namespace"));
        XML_OPTIONAL(config->cpp.cmakeExport = doc.GetNode("/config/cpp/cmake").GetStringProp("export"));

        XML_OPTIONAL(config->cpp.writeTimestamp = doc.GetNode("/config/cpp/timestamp").GetBoolProp("enable"));

        try
        {
            auto pfNodes = doc.GetNodes("/config/cpp/portFilter/port");
            for (const auto& pfNode : pfNodes)
            {
                config->cpp.portFilter.push_back(pfNode.GetStringProp("name"));
            }
        }
        catch(const std::exception& e)
        {
        }

        XML_OPTIONAL(config->cpp.enableHacks = doc.GetNode("/config/cpp/hacks").GetBoolProp("enable"));
        try
        {
            auto tpNodes = doc.GetNodes("/config/cpp/hacks/ignored-wsdl-types/operation");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredOperations.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch(const std::exception& e)
        {
        }
        try
        {
            auto tpNodes = doc.GetNodes("/config/cpp/hacks/ignored-wsdl-types/message");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredMessages.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch(const std::exception& e)
        {
        }
        try
        {
            auto tpNodes = doc.GetNodes("/config/cpp/hacks/ignored-wsdl-types/type");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredTypes.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch(const std::exception& e)
        {
        }


        return config;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return {};
}

static void UpdateConfig(
    Config& config,
    argparse::ArgumentParser& program)
{
    auto outPath = program.get<std::string>("--out");
    if (!outPath.empty())
    {
        config.cpp.outputPath = outPath;
    }
}


int main(
    int argc,
    const char** argv)
{
    argparse::ArgumentParser program{"soapgen"};

    program.add_argument("config")
        .help("location of the config file")
        .default_value(std::string{"config.xml"})
        .required();

    program.add_argument("--out")
        .help("output path")
        .default_value(std::string{""});

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return -1;
    }

    auto config = LoadConfig(program.get<std::string>("config"));

    if (!config)
    {
        std::cerr << "could not load config" << '\n';
        return -1;
    }

    UpdateConfig(*config, program);

    auto definition = LoadWsdl(config->wsdlFile);

    if (!definition)
    {
        std::cerr << "could not load wsdl" << '\n';
        return -1;
    }

    cppgen::Generate(config->cpp, *definition);

    return 0;
}
