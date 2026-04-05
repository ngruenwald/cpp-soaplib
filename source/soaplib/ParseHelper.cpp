#include "ParseHelper.hpp"

namespace soaplib {

bool IsValidNode(
    const xml::Node& node)
{
    return node.GetXmlNode() != nullptr;
}

bool IsNil(
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

void ResetPrefix()
{
    prefix[0] = 'a';
    prefix[1] = 'a';
    prefix[2] = 'a';
}

std::string GeneratePrefix()
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
                ResetPrefix();
            }
        }
    }

    return cur;
}

void AddNamespace(
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
        pre = GeneratePrefix();
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

xml::Node AddChild(
    xml::Document& doc,
    xml::Node& parentNode,
    const std::string& name,
    const std::string& href,
    const std::string& prefix)
{
    auto node = parentNode.AddChild(name.c_str());
    if (!href.empty() || !prefix.empty())
    {
        AddNamespace(doc, node, href, prefix);
    }
    return node;
}

void SetAnyTypeAttribute(
    xml::Document& doc,
    xml::Node& anyNode,
    const std::string& type,
    const std::string& href,
    const std::string& prefix)
{

    const auto xmlnsName = std::string{"xmlns:"} + prefix;
    const auto fullType = prefix + ":" + type;
    //AddNamespace(doc, anyNode, href, prefix);
    anyNode.SetProp("i:type", fullType.c_str());
    anyNode.SetProp(xmlnsName.c_str(), href.c_str());
}

} // namespace soaplib
