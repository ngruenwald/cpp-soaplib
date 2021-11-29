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

std::string generateRandomPrefix()
{
    const int length = 3;

    std::string prefix;

    for (int idx = 0; idx < length; ++idx)
    {
        prefix += 'a' + (char)(rand() % ('z' - 'a'));
    }

    return prefix;
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
        pre = generateRandomPrefix();
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

} // namespace soaplib
