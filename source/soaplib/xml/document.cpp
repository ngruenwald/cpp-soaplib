#include "document.hpp"
#include "exception.hpp"

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <sstream>

namespace soaplib {
namespace xml {

inline const char* ascii_cast(const xmlChar* pxc)
{
    // This "kills" UTF-8 and can produce garbage
    return reinterpret_cast<const char*>(pxc);
}

auto evaluateXPath(
    xmlXPathContextPtr xpathContext,
    const char* xpath)
{
    xpathContext->node = nullptr;
    return
        std::unique_ptr<xmlXPathObject, decltype(&xmlXPathFreeObject)>
        {
            xmlXPathEvalExpression(BAD_CAST xpath, xpathContext),
            &xmlXPathFreeObject
        };
}


Document::Document()
    : doc_{xmlNewDoc(BAD_CAST "1.0")}
    , xpathContext_{xmlXPathNewContext(doc_)}
{
}

Document::Document(
    const char* fileName)
    : doc_{xmlReadFile(fileName, nullptr, 0)}
    , xpathContext_{xmlXPathNewContext(doc_)}
{
}

Document::Document(
    const char* buffer,
    size_t length)
    : doc_{xmlReadMemory(buffer, static_cast<int>(length), "memory.xsd", nullptr, 0)}
    , xpathContext_{xmlXPathNewContext(doc_)}
{
}

Document::~Document()
{
    xmlFreeDoc(doc_);
    xmlXPathFreeContext(xpathContext_);
}

Node Document::GetRootNode()
{
    auto rootNode = xmlDocGetRootElement(doc_);

    if (rootNode == nullptr)
    {
        throw xml::Exception{"RootNode does not exist, document is empty"};
    }

    return Node{rootNode};
}

Node Document::CreateRootNode(const char* name)
{
    if (xmlDocGetRootElement(doc_) != nullptr)
    {
        throw xml::Exception{"RootNode already exists"};
    }

    auto rootNode = xmlNewNode(nullptr, BAD_CAST name);

    if (rootNode == nullptr)
    {
        throw xml::Exception{"Coult not create RootNode"};
    }

    xmlDocSetRootElement(doc_, rootNode);

    return Node{rootNode};
}

std::string Document::Serialize(
    const char* encoding,
    bool pretty) const
{
    xmlBuffer* buffer = xmlBufferCreate();
    xmlOutputBuffer* outputBuffer = xmlOutputBufferCreateBuffer(buffer, nullptr);

    int res = xmlSaveFormatFileTo(outputBuffer, doc_, encoding, pretty ? 1 : 0);

    if (res == -1)
    {
        xmlBufferFree(buffer);
        throw xml::Exception{"Could not save to buffer"};
    }

    std::string result(ascii_cast(buffer->content), buffer->use);

    xmlBufferFree(buffer);

    return result;
}

xmlDocPtr Document::GetXmlDoc() const
{
    return doc_;
}

const Node Document::GetNode(
    const char* xpath) const
{
    auto xpathObj = evaluateXPath(xpathContext_, xpath);

    if (xmlXPathNodeSetIsEmpty(xpathObj->nodesetval))
    {
        std::ostringstream oss;
        oss << "Node \"" << xpath << "\" not found";
        throw xml::Exception{oss.str()};
    }

    return Node{xmlXPathNodeSetItem(xpathObj->nodesetval, 0)};
}

NodeList Document::GetNodes(
    const char* xpath) const
{
    auto xpathObj = evaluateXPath(xpathContext_, xpath);

    NodeList nodes;

    for (int idx = 0; idx < xmlXPathNodeSetGetLength(xpathObj->nodesetval); idx++)
    {
        nodes.push_back(Node{xmlXPathNodeSetItem(xpathObj->nodesetval, idx)});
    }

    return nodes;
}

Document Document::ParseFile(
    const char* fileName)
{
    return Document{fileName};
}

Document Document::ParseMemory(
    const char* buffer,
    size_t length)
{
    return Document{buffer, length};
}

} // namespace xml
} // namespace soaplib
