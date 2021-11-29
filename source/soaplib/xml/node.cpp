#include "node.hpp"
#include "exception.hpp"

#include <string.h>
#include <sstream>

namespace soaplib {
namespace xml {

Node::Node()
{
}

Node::Node(xmlNodePtr node)
    : node_{node}
{
}

Node::~Node()
{
    /*xmlFreeNode(node_);*/
}

bool Node::IsValid() const
{
    return node_ != nullptr;
}

const char* Node::GetName() const
{
    return reinterpret_cast<const char*>(node_->name);
}

void Node::SetProp(
    const char* name,
    const char* value)
{
    auto prop = xmlSetProp(node_, BAD_CAST name, BAD_CAST value);

    if (prop == nullptr)
    {
        std::ostringstream oss;
        oss << "Could not set property \"" << name << "\"";
        throw xml::Exception{oss.str()};
    }
}

void Node::SetProp(
    const char* name,
    int value)
{
    auto s = std::to_string(value);
    SetProp(name, s.c_str());
}

void Node::SetProp(
    const char* name,
    long long value)
{
    auto s = std::to_string(value);
    SetProp(name, s.c_str());
}

void Node::SetProp(
    const char* name,
    unsigned int value)
{
    auto s = std::to_string(value);
    SetProp(name, s.c_str());
}

void Node::SetProp(
    const char* name,
    unsigned long long value)
{
    auto s = std::to_string(value);
    SetProp(name, s.c_str());
}

void Node::SetVal(
    const char* value)
{
    xmlNodeSetContent(node_, BAD_CAST value);
}

void Node::SetVal(
    const std::string& value)
{
    SetVal(value.c_str());
}

std::string Node::GetStringProp(
    const char* name) const
{
    const auto prop = xmlGetProp(node_, BAD_CAST name);

    if (prop == nullptr)
    {
        std::ostringstream oss;
        oss << "Could not get property \"" << name << "\"";
        throw xml::Exception{oss.str()};
    }

    auto result = std::string{reinterpret_cast<const char*>(prop)};
    xmlFree(prop);
    return result;
}

int Node::GetIntProp(
    const char* name) const
{
    return std::stoi(GetStringProp(name));
}

bool Node::GetBoolProp(
    const char* name) const
{
    return strcasecmp("true", GetStringProp(name).c_str()) == 0;
}

std::string Node::GetStringVal() const
{
    const auto content = xmlNodeGetContent(node_);

    if (content == nullptr)
    {
        return {};  // TODO: throw?
    }

    auto result = std::string{reinterpret_cast<const char*>(content)};
    xmlFree(content);
    return result;
}

int Node::GetIntVal() const
{
    return std::stoi(GetStringVal());
}

Node Node::AddChild(
    const char* name)
{
    return Node{xmlNewChild(node_, nullptr, BAD_CAST name, nullptr)};
}

Node Node::GetChild(
    const char* name) const
{
    for (auto xmlNode = xmlFirstElementChild(node_);
         xmlNode != nullptr;
         xmlNode = xmlNextElementSibling(xmlNode))
    {
        if (xmlStrcmp(xmlNode->name, BAD_CAST name) == 0)
        {
            return Node{xmlNode};
        }
    }

    std::ostringstream oss;
    oss << "Child node \"" << name << "\" not found";
    throw xml::Exception(oss.str());
}

std::vector<Node> Node::GetChildren(
    const char* name) const
{
    std::vector<Node> nodes;

    for (auto xmlNode = xmlFirstElementChild(node_);
         xmlNode != nullptr;
         xmlNode = xmlNextElementSibling(xmlNode))
    {
        if (name == nullptr || xmlStrcmp(xmlNode->name, BAD_CAST name) == 0)
        {
            nodes.emplace_back(xmlNode);
        }
    }

    return nodes;
}

xmlNodePtr Node::GetXmlNode() const
{
    return node_;
}

} // namespace xml
} // namespace soaplib
