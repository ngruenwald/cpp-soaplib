#include "unit_tests.hpp"

#include <fstream>

#include <soaplib/xml/document.hpp>
#include <soaplib/xml/node.hpp>
#include <soaplib/xml/xml.hpp>


std::string loadFile(const std::string path)
{
    auto ifs = std::ifstream{path.c_str(), std::ios::binary | std::ios::ate};
    REQUIRE(ifs.is_open());
    auto size = ifs.tellg();
    std::string buffer;//{static_cast<std::size_t>(size), '\0'};
    buffer.resize(static_cast<std::size_t>(size));
    ifs.seekg(0);
    ifs.read(&buffer[0], size);
    return buffer;
}


TEST_CASE("xml load document", "[all][xml]")
{
    const std::string fixture{FIXTURES_PATH "/simple.xml"};
    auto buffer = loadFile(fixture);
    REQUIRE(!buffer.empty());

    auto doc = soaplib::xml::Document::ParseMemory(buffer.c_str(), buffer.length());
    auto s2 = doc.Serialize();

    REQUIRE(buffer == s2);
}

TEST_CASE("xml nodes", "[all][xml")
{
    const std::string fixture{FIXTURES_PATH "/simple.xml"};
    auto buffer = loadFile(fixture);
    REQUIRE(!buffer.empty());
    auto doc = soaplib::xml::Document::ParseMemory(buffer.c_str(), buffer.length());

    std::string name;

    auto root = doc.GetRootNode();
    REQUIRE(root.IsValid());
    name = root.GetName();
    REQUIRE(name == "root");

    auto single = root.GetChild("single");
    REQUIRE(single.IsValid());
    name = single.GetName();
    REQUIRE(name == "single");

    auto list = root.GetChild("list");
    REQUIRE(list.IsValid());
    name = list.GetName();
    REQUIRE(name == "list");

    auto entries = list.GetChildren("entry");
    REQUIRE(entries.size() == 2);
    for (const auto& entry : entries)
    {
        name = entry.GetName();
        REQUIRE(name == "entry");
    }
}

/*
struct Single
{
    bool Valid = false;
};

void from_xml(soaplib::xml::Node& n, Single& v)
{
    v.Valid = true;
}

TEST_CASE("xml utils", "[all][xml")
{
    const std::string fixture{FIXTURES_PATH "/simple.xml"};
    auto buffer = loadFile(fixture);
    REQUIRE(!buffer.empty());
    auto doc = soaplib::xml::Document::ParseMemory(buffer.c_str(), buffer.length());

    auto root = doc->GetRootNode();

    auto single = soaplib::xml::from_xml<Single>(root, "single");
}
*/
