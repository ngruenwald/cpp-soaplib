#include "Document.hpp"
#include "Exception.hpp"

#include <libxml/tree.h>
#include <libxml/xpath.h>

#include <sstream>
#include <utility>

namespace soaplib::xml {

inline const char* ascii_cast(
    const xmlChar* pxc)
{
    // This "kills" UTF-8 and can produce garbage
    return reinterpret_cast<const char*>(pxc);
}

inline const char* get_content(_xmlBuffer* buffer)
{
    return ascii_cast(xmlBufferContent(buffer));
}

inline size_t get_content_length(_xmlBuffer* buffer)
{
    return xmlBufferLength(buffer);
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

Document::Document(Document&& other) noexcept
    : doc_{std::exchange(other.doc_, nullptr)}
    , xpathContext_{std::exchange(other.xpathContext_, nullptr)}
{
}

Document& Document::operator=(Document&& other) noexcept
{
    if (this != &other)
    {
        if (doc_) xmlFreeDoc(doc_);
        if (xpathContext_) xmlXPathFreeContext(xpathContext_);
        
        doc_ = std::exchange(other.doc_, nullptr);
        xpathContext_ = std::exchange(other.xpathContext_, nullptr);
    }
    return *this;
}

Document::~Document()
{
    if (doc_) xmlFreeDoc(doc_);
    if (xpathContext_) xmlXPathFreeContext(xpathContext_);
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

Node Document::CreateRootNode(
    const char* name)
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

    std::string result(get_content(buffer), get_content_length(buffer));

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

std::unique_ptr<Document> Document::ParseFile(
    const char* fileName)
{
    return std::unique_ptr<Document>(new Document(fileName));
}

std::unique_ptr<Document> Document::ParseMemory(
    const char* buffer,
    size_t length)
{
    return std::unique_ptr<Document>(new Document(buffer, length));
}

} // namespace soaplib::xml
