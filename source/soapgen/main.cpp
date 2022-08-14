#include <iostream>
#include "wsdl.hpp"
#include "cppgen/cppgen.hpp"

#include "soaplib/xml/xml.hpp"

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


int main(int argc, const char** argv)
{
    const char* configFile = "config.xml";

    if (argc > 1)
    {
        configFile = argv[1];
    }

    auto config = LoadConfig(configFile);

    if (!config)
    {
        std::cerr << "could not load config" << '\n';
        return -1;
    }

    auto definition = LoadWsdl(config->wsdlFile);

    if (!definition)
    {
        std::cerr << "could not load wsdl" << '\n';
        return -1;
    }

    cppgen::Generate(config->cpp, *definition);

    return 0;
}
