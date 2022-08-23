#include "parseHelper.hpp"

namespace soaplib {

bool isValidNode(
    const xml::Node& node)
{
    return node.GetXmlNode() != nullptr;
}

bool isNil(
    const xml::Node& node)
{
    try
    {
        return node.GetBoolProp("s:nil"); // TODO: use correct namespace
    }
    catch(const xml::Exception&)
    {
        return false;
    }
}

static char prefix[] = { 'a', 'a', 'a', '\0' };

void resetPrefix()
{
    prefix[0] = 'a';
    prefix[1] = 'a';
    prefix[2] = 'a';
}

std::string generatePrefix()
{
    std::string cur{prefix};

    prefix[2] += 1;

    if (prefix[2] > 'z')
    {
        prefix[2] = 'a';
        prefix[1] += 1;

        if (prefix[1] > 'z')
        {
            prefix[1] = 'a';
            prefix[0] += 1;

            if (prefix[0] > 'z')
            {
                // oh, oh!
                resetPrefix();
            }
        }
    }

    return cur;
}

void addNamespace(
    xml::Document& doc,
    xml::Node& node,
    const std::string& href,
    const std::string& prefix)
{
    auto np = node.GetXmlNode();

    std::string pre = prefix;
    bool prefixExists = false;

    // search for a matching prefix
    if (!prefix.empty())
    {
        auto ns = xmlSearchNs(doc.GetXmlDoc(), np, BAD_CAST prefix.c_str());
        if (ns)
        {
            prefixExists = true;

            // only apply if the href is equal
            if (href.compare((const char*)ns->href) == 0)
            {
                xmlSetNs(np, ns);
                return;
            }
        }
    }

    // search for a matching href
    if (!href.empty())
    {
        auto ns = xmlSearchNsByHref(doc.GetXmlDoc(), np, BAD_CAST href.c_str());
        if (ns)
        {
            xmlSetNs(np, ns);
            return;
        }
    }

    if (href.empty() || prefix.empty())
    {
        return;
    }

    while (prefixExists)
    {
        pre = generatePrefix();
        auto ns = xmlSearchNs(doc.GetXmlDoc(), np, BAD_CAST pre.c_str());
        if (ns)
        {
            // only apply if the href is equal
            if (href.compare((const char*)ns->href) == 0)
            {
                xmlSetNs(np, ns);
                return;
            }
        }
        else
        {
            prefixExists = false;
        }
    }

    xmlSetNs(np, xmlNewNs(np, BAD_CAST href.c_str(), BAD_CAST pre.c_str()));
}

xml::Node addChild(
    xml::Document& doc,
    xml::Node& parentNode,
    const std::string& name,
    const std::string& href,
    const std::string& prefix)
{
    auto node = parentNode.AddChild(name.c_str());
    if (!href.empty() || !prefix.empty())
    {
        addNamespace(doc, node, href, prefix);
    }
    return node;
}

void setAnyTypeAttribute(
    xml::Document& doc,
    xml::Node& anyNode,
    const std::string& type,
    const std::string& href,
    const std::string& prefix)
{

    const auto xmlnsName = std::string{"xmlns:"} + prefix;
    const auto fullType = prefix + ":" + type;
    //addNamespace(doc, anyNode, href, prefix);
    anyNode.SetProp("i:type", fullType.c_str());
    anyNode.SetProp(xmlnsName.c_str(), href.c_str());
}

} // namespace soaplib
