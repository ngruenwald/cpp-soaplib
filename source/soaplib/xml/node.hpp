#pragma once

#include <string>
#include <vector>

#include <libxml/tree.h>

namespace soaplib {
namespace xml {

class Document;

class Node
{
    friend class Document;

public:
    Node();
    Node(xmlNodePtr node);

    bool IsValid() const;

public:
    virtual ~Node();

    const char* GetName() const;

public:
    void SetProp(const char* name, const char* value);

    void SetProp(const char* name, int value);
    void SetProp(const char* name, long long value);

    void SetProp(const char* name, unsigned int value);
    void SetProp(const char* name, unsigned long long value);

    void SetVal(const char* value);
    void SetVal(const std::string& value);


    std::string GetStringProp(const char* name) const;
    int GetIntProp(const char* name) const;
    bool GetBoolProp(const char* name) const;

    std::string GetStringVal() const;
    int GetIntVal() const;

public:
    Node AddChild(const char* name);
    Node GetChild(const char* name) const;

    std::vector<Node> GetChildren(const char* name) const;

    xmlNodePtr GetXmlNode() const;

private:
    xmlNodePtr node_{nullptr};
};

typedef std::vector<Node> NodeList;

} // namespace xml
} // namespace soaplib
