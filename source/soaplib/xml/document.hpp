#pragma once

#include <memory>
#include <string>

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <soaplib/xml/node.hpp>

namespace soaplib {
namespace xml {

/// XML document
class Document
{
private:
    explicit Document(
        const char* fileName);

public:
    /// Default constructor
    Document();

    /// Constructs a XML document from the given data
    /// @param[in] buffer Pointer to XML data
    /// @param[in] length Size of the XML data
    Document(
        const char* buffer,
        size_t length);

    /// No copy construction
    Document(
        const Document&) = delete;

    /// No copying
    Document& operator=(
        const Document&) = delete;

    /// Destructor
    virtual ~Document();

public:
    /// Returns the root node
    /// @returns XML node
    Node GetRootNode();

    /// Creates a root node
    /// @param[in] name The name of the root element
    /// @returns XML node
    Node CreateRootNode(
        const char* name);

    /// Serializes the XML document as string
    /// @param[in] encoding The encoding of the serialized data
    /// @param[in] pretty If true, makes the output more readable
    std::string Serialize(
        const char* encoding = "UTF-8",
        bool pretty = true) const;

public:
    /// Returns the internal libxml2 data
    /// @returns libxml2 document pointer
    xmlDocPtr GetXmlDoc() const;

    /// Gets the node matching the given path
    /// @param[in] xpath XML path
    /// @returns XML node
    const Node GetNode(
        const char* xpath) const;

    /// Gets all the nodes matching the given path
    /// @param[in] xpath XML path
    /// @returns List of XML nodes
    NodeList GetNodes(
        const char* xpath) const;

public:
    /// Loads XML from file
    /// @param[in] fileName Path of the XML file to load
    /// @returns XML document
    static Document ParseFile(
        const char* fileName);

    /// Loads XML from memory
    /// @param[in] buffer Pointer to XML data
    /// @param[in] length Size of the XML data
    /// @returns XML document
    static Document ParseMemory(
        const char* buffer,
        size_t length);

private:
    xmlDocPtr doc_{nullptr};
    xmlXPathContextPtr xpathContext_{nullptr};
};

} // namespace xml
} // namespace soaplib
