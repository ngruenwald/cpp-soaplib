#pragma once

#include <string>

#include <pugixml.hpp>

struct Echo
{
	std::optional<std::string> value;
};

void to_xml(
	pugi::xml_node& parent,
	const struct Echo& v,
	pugi::xml_node& result)
{	
	result = 
		parent.append_child("Echo")
		      .append_child("value")
			  .append_child(pugi::node_pcdata)
			  .set_value(v.value);
}

void from_xml(
	pugi::xml_node& node,
	struct Echo& v)
{
	v.value = node.first_child().value();
}

struct EchoResponse
{
	std::optional<std::string> value;
};

void to_xml(
	pugi::xml_node& parent,
	const struct EchoResponse& v,
	pugi::xml_node& result)
{
	result =
		parent.append_child("Echo")
		.append_child("value")
		.append_child(pugi::node_pcdata)
		.set_value(v.value);
}

void from_xml(
	pugi::xml_node& node,
	struct EchoResponse& v)
{
	v.value = node.first_child().value();
}

struct LogOnDCRequest
{

};

struct LogonBasic
{
	LogOnDCRequest logonRequest;
};

