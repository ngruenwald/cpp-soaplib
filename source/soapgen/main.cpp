#include <iostream>
#include <argparse/argparse.hpp>
#include "cppgen/cppgen.hpp"
#include "wsdl.hpp"

#include "soaplib/xml/Xml.hpp"

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

        XML_MANDATORY(config->wsdlFile = doc->GetNode("/config/wsdl").GetStringProp("path"));

        XML_OPTIONAL(config->cpp.name = doc->GetNode("/config").GetStringProp("name"));
        XML_OPTIONAL(config->cpp.outputPath = doc->GetNode("/config/cpp/output").GetStringProp("path"));
        XML_OPTIONAL(config->cpp.appendNamespacesToPath = doc->GetNode("/config/cpp/output").GetBoolProp("append-ns"));
        XML_OPTIONAL(config->cpp.typesSubfolder = doc->GetNode("/config/cpp/output").GetStringProp("types-subfolder"));

        XML_OPTIONAL(config->cpp.generateClient = doc->GetNode("/config/cpp/client").GetBoolProp("enable"));
        XML_OPTIONAL(config->cpp.generateServer = doc->GetNode("/config/cpp/server").GetBoolProp("enable"));
        XML_OPTIONAL(config->cpp.templatePath = doc->GetNode("/config/cpp/template-path").GetStringProp("path"));
        XML_OPTIONAL(config->cpp.abortOnUnknownType = doc->GetNode("/config/cpp/abort-on-unknown").GetBoolProp("enable"));

        try {
            auto v = doc->GetNode("/config/cpp/soap-version").GetStringVal();
            if (v == "1.1") config->cpp.soapVersion = cppgen::Options::SoapVersion::Soap11;
            else if (v == "1.2") config->cpp.soapVersion = cppgen::Options::SoapVersion::Soap12;
        } catch (...) {}

        try
        {
            auto nsNodes = doc->GetNodes("/config/cpp/namespaces/namespace");
            for (const auto& nsNode : nsNodes)
            {
                config->cpp.namespaces.push_back(nsNode.GetStringVal());
            }
        }
        catch (const std::exception& e)
        {
        }

        try
        {
            auto ptrNodes = doc->GetNodes("/config/cpp/pointer-types/param");
            for (const auto& ptrNode : ptrNodes)
            {
                config->cpp.pointerTypes.push_back(ptrNode.GetStringVal());
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }


        XML_OPTIONAL(config->cpp.cmakeNamespace = doc->GetNode("/config/cpp/cmake").GetStringProp("namespace"));
        XML_OPTIONAL(config->cpp.cmakeExport = doc->GetNode("/config/cpp/cmake").GetStringProp("export"));

        XML_OPTIONAL(config->cpp.writeTimestamp = doc->GetNode("/config/cpp/timestamp").GetBoolProp("enable"));

        try
        {
            auto pfNodes = doc->GetNodes("/config/cpp/portFilter/port");
            for (const auto& pfNode : pfNodes)
            {
                config->cpp.portFilter.push_back(pfNode.GetStringProp("name"));
            }
        }
        catch (const std::exception& e)
        {
        }

        XML_OPTIONAL(config->cpp.enableHacks = doc->GetNode("/config/cpp/hacks").GetBoolProp("enable"));
        try
        {
            auto tpNodes = doc->GetNodes("/config/cpp/hacks/ignored-wsdl-types/operation");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredOperations.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch (const std::exception& e)
        {
        }
        try
        {
            auto tpNodes = doc->GetNodes("/config/cpp/hacks/ignored-wsdl-types/message");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredMessages.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch (const std::exception& e)
        {
        }
        try
        {
            auto tpNodes = doc->GetNodes("/config/cpp/hacks/ignored-wsdl-types/type");
            for (const auto& tpNode : tpNodes)
            {
                config->cpp.ignoredTypes.push_back(tpNode.GetStringProp("name"));
            }
        }
        catch (const std::exception& e)
        {
        }


        return config;
    }
    catch (const std::exception& e)
    {
        //std::cerr << e.what() << '\n';
    }

    return {};
}


int main(int argc, const char** argv)
{
    argparse::ArgumentParser program("soapgen", "0.2.0");

    program.add_argument("config")
        .help("path to config.xml file")
        .remaining()
        .default_value(std::string("config.xml"));

    program.add_argument("-i", "--input")
        .help("path to the input wsdl file");

    program.add_argument("-o", "--output")
        .help("output path for generated files");

    program.add_argument("-n", "--namespace")
        .help("namespace(s) to use")
        .append();

    program.add_argument("--client")
        .help("enable generation of client proxy")
        .implicit_value(true);

    program.add_argument("--server")
        .help("enable generation of server stubs")
        .implicit_value(true);

    program.add_argument("--abort-on-unknown")
        .help("abort if an unknown type is detected")
        .implicit_value(true);

    program.add_argument("--soap-version")
        .help("SOAP version (1.1, 1.2, auto)");

    program.add_argument("--types-folder")
        .help("subfolder for types");

    program.add_argument("--template-path")
        .help("path to template files");

    program.add_argument("--cmake-namespace")
        .help("namespace for generated CMake targets");

    program.add_argument("--cmake-export")
        .help("name for CMake export");

    try
    {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        return 1;
    }

    auto configPath = program.get<std::string>("config");
    auto config = LoadConfig(configPath);

    if (!config)
    {
        if (program.present("--input"))
        {
            config = std::make_unique<Config>();
        }
        else
        {
            std::cerr << "could not load config file '" << configPath << "' and no --input provided." << '\n';
            return -1;
        }
    }

    // CLI overrides
    if (auto v = program.present("--input")) config->wsdlFile = *v;
    if (auto v = program.present("--output")) config->cpp.outputPath = *v;
    if (auto v = program.present("--types-folder")) config->cpp.typesSubfolder = *v;
    if (auto v = program.present("--template-path")) config->cpp.templatePath = *v;
    if (auto v = program.present("--cmake-namespace")) config->cpp.cmakeNamespace = *v;
    if (auto v = program.present("--cmake-export")) config->cpp.cmakeExport = *v;

    if (program.present("--client")) config->cpp.generateClient = true;
    if (program.present("--server")) config->cpp.generateServer = true;
    if (program.present("--abort-on-unknown")) config->cpp.abortOnUnknownType = true;

    if (auto v = program.present("--soap-version")) {
        if (*v == "1.1") config->cpp.soapVersion = cppgen::Options::SoapVersion::Soap11;
        else if (*v == "1.2") config->cpp.soapVersion = cppgen::Options::SoapVersion::Soap12;
        else if (*v == "auto") config->cpp.soapVersion = cppgen::Options::SoapVersion::Auto;
    }

    if (auto v = program.present<std::vector<std::string>>("--namespace"))
    {
        if (!v->empty())
        {
            config->cpp.namespaces = *v;
        }
    }

    if (config->wsdlFile.empty())
    {
        std::cerr << "No WSDL input file specified." << std::endl;
        return -1;
    }

    soapgen::ResourceResolver resolver;
    auto definition = LoadWsdl(config->wsdlFile, resolver);

    if (!definition)
    {
        std::cerr << "could not load wsdl: " << config->wsdlFile << '\n';
        return -1;
    }

    try
    {
        cppgen::Generate(config->cpp, *definition);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Generation failed: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
