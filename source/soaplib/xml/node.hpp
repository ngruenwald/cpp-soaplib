#pragma once

#include <string>
#include <vector>

#include <libxml/tree.h>

namespace soaplib {
namespace xml {

class Document;

/// XML node
class Node
{
    friend class Document;

public:
    /// Default constructor
    Node();

    /// Constructs node object from libxml2 node pointer
    Node(
        xmlNodePtr node);

    /// Checks if the contained node pointer is valid
    /// @returns True if the node is valid, otherwise false
    bool IsValid() const;

public:
    /// Destructor
    virtual ~Node();

    /// Gets the name of the node
    /// @returns String containing the node name
    const char* GetName() const;

public:
    /// Sets an XML attribute (string)
    /// @param[in] name The name of the attribute
    /// @param[in] value The attribute value
    void SetProp(
        const char* name,
        const char* value);

    /// Sets an XML attribute (int)
    /// @param[in] name The name of the attribute
    /// @param[in] value The attribute value
    void SetProp(const char* name, int value);

    /// Sets an XML attribute (long long)
    /// @param[in] name The name of the attribute
    /// @param[in] value The attribute value
    void SetProp(const char* name, long long value);

    /// Sets an XML attribute (unsigned int)
    /// @param[in] name The name of the attribute
    /// @param[in] value The attribute value
    void SetProp(const char* name, unsigned int value);

    /// Sets an XML attribute (unsigned long long)
    /// @param[in] name The name of the attribute
    /// @param[in] value The attribute value
    void SetProp(const char* name, unsigned long long value);

    /// Sets the node content
    /// @param[in] value The value
    void SetVal(const char* value);

    /// Sets the node content
    /// @param[in] value The value
    void SetVal(const std::string& value);

    /// Gets an XML attribute (string)
    /// @param[in] name The name of the attribute
    /// @returns The attribute value
    std::string GetStringProp(const char* name) const;

    /// Gets an XML attribute (int)
    /// @param[in] name The name of the attribute
    /// @returns The attribute value
    int GetIntProp(const char* name) const;

    /// Gets an XML attribute (bool)
    /// @param[in] name The name of the attribute
    /// @returns The attribute value
    bool GetBoolProp(const char* name) const;


    /// Gets the node content (string)
    /// @returns The value
    std::string GetStringVal() const;

    /// Gets the node content (int)
    /// @returns The value
    int GetIntVal() const;

public:
    /// Add a child node
    /// @param[in] name The name of the child element
    /// @returns The child node
    Node AddChild(const char* name);

    /// Gets a child node
    /// @param[in] name The name of the child element
    /// @returns The child node
    Node GetChild(const char* name) const;

    /// Gets a list of child nodes
    /// @param[in] name The name of the child elements
    /// @returns The child nodes
    std::vector<Node> GetChildren(const char* name) const;

    /// Returns the internal libxml2 data
    /// @returns libxml2 node pointer
    xmlNodePtr GetXmlNode() const;

private:
    xmlNodePtr node_{nullptr};
};

typedef std::vector<Node> NodeList; ///< List of XML nodes

} // namespace xml
} // namespace soaplib
