#pragma once

#include <memory>
#include <string>

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <soaplib/xml/node.hpp>

namespace soaplib {
namespace xml {

class Document
{
private:
    explicit Document(const char* fileName);

public:
    Document();

    Document(const char* buffer, size_t length);

    Document(const Document&) = delete;
    Document& operator=(const Document&) = delete;

    virtual ~Document();

public:
    Node GetRootNode();

    Node CreateRootNode(const char* name);

    std::string Serialize(
        const char* encoding = "UTF-8",
        bool pretty = true) const;

public:
    xmlDocPtr GetXmlDoc() const;

    const Node GetNode(
        const char* xpath) const;

    NodeList GetNodes(
        const char* xpath) const;

public:
    static Document ParseFile(
        const char* fileName);

    static Document ParseMemory(
        const char* buffer,
        size_t length);

private:
    xmlDocPtr doc_{nullptr};
    xmlXPathContextPtr xpathContext_{nullptr};
};

} // namespace xml
} // namespace soaplib
