#include "unit_tests.hpp"
#include <cppgen/cppgen.hpp>
#include <wsdl.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

bool compareFiles(const fs::path& p1, const fs::path& p2)
{
    std::ifstream f1(p1, std::ios::binary);
    std::ifstream f2(p2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open())
    {
        return false;
    }

    std::string s1((std::istreambuf_iterator<char>(f1)), std::istreambuf_iterator<char>());
    std::string s2((std::istreambuf_iterator<char>(f2)), std::istreambuf_iterator<char>());

    return s1 == s2;
}

void compareDirectories(const fs::path& generatedDir, const fs::path& goldenDir)
{
    for (const auto& entry : fs::recursive_directory_iterator(goldenDir))
    {
        if (entry.is_directory()) continue;

        fs::path relativePath = fs::relative(entry.path(), goldenDir);
        fs::path generatedPath = generatedDir / relativePath;

        INFO("Comparing file: " << relativePath.string());
        REQUIRE(fs::exists(generatedPath));
        
        if (!compareFiles(entry.path(), generatedPath))
        {
            // If they are different, print them for debugging (optional, but helpful)
            // std::cout << "Difference in " << relativePath << std::endl;
            REQUIRE(compareFiles(entry.path(), generatedPath));
        }
    }
}

TEST_CASE("Generator: Calculator", "[all][generator]")
{
    std::string wsdlPath = FIXTURES_PATH "/../../examples/calculator/calculator.wsdl";
    std::string goldenDir = FIXTURES_PATH "/golden/calc";
    fs::path tempDir = fs::path{OUTPUT_BASE_PATH} / "soapgen_test_calc";

    if (fs::exists(tempDir)) fs::remove_all(tempDir);
    fs::create_directories(tempDir);

    cppgen::Options options;
    options.name = "calculator-service";
    options.outputPath = tempDir.string();
    options.appendNamespacesToPath = false;
    options.typesSubfolder = "types";
    options.namespaces = {"calc"};
    options.cmakeNamespace = "calc";
    options.templatePath = TEMPLATES_PATH;
    options.writeTimestamp = false;
    options.portFilter = {"CalculatorSoap"};

    auto definition = LoadWsdl(wsdlPath);
    REQUIRE(definition);

    cppgen::Generate(options, *definition);

    compareDirectories(tempDir, goldenDir);

    // fs::remove_all(tempDir);
}

TEST_CASE("Generator: CountryInfo", "[all][generator]")
{
    std::string wsdlPath = FIXTURES_PATH "/../../examples/country/CountryInfoService.wsdl";
    std::string goldenDir = FIXTURES_PATH "/golden/country";
    fs::path tempDir = fs::path{OUTPUT_BASE_PATH} / "soapgen_test_country";

    if (fs::exists(tempDir)) fs::remove_all(tempDir);
    fs::create_directories(tempDir);

    cppgen::Options options;
    options.name = "country-info";
    options.outputPath = tempDir.string();
    options.appendNamespacesToPath = false;
    options.typesSubfolder = "types";
    options.namespaces = {"country"};
    options.cmakeNamespace = "country";
    options.templatePath = TEMPLATES_PATH;
    options.writeTimestamp = false;
    options.portFilter = {"CountryInfoServiceSoap12"};

    auto definition = LoadWsdl(wsdlPath);
    REQUIRE(definition);

    cppgen::Generate(options, *definition);

    compareDirectories(tempDir, goldenDir);

    // fs::remove_all(tempDir);
}
