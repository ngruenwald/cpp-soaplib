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

void addNamespace(
	xml::Document& doc,
	xml::Node& node,
	const std::string& href,
	const std::string& prefix)
{
	auto np = node.GetXmlNode();

    if (!prefix.empty())
    {
        auto ns = xmlSearchNs(doc.GetDoc(), np, BAD_CAST prefix.c_str());
		if (ns)
		{
			xmlSetNs(np, ns);
            return;
		}
    }

	if (!href.empty())
	{
		auto ns = xmlSearchNsByHref(doc.GetDoc(), np, BAD_CAST href.c_str());
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

	xmlSetNs(np, xmlNewNs(np, BAD_CAST href.c_str(), BAD_CAST prefix.c_str()));
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
