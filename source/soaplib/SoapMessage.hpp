#pragma once

#include <string>

namespace soaplib {

/// SOAP message header
struct Header
{
    /// Action element of SOAP header
    struct Action
    {
        std::string value;          ///< Action name

        // attributes
        int mustUnderstand = 1;     ///< If set, receiver must understand this element
    };

    /// MessageID element of SOAP header
    struct MessageID
    {
        std::string value;          ///< URN
    };

    /// ReplyTo element of SOAP header
    struct ReplyTo
    {
        /// Address element of ReplyTo header element
        struct Address
        {
            std::string value;      ///< addressNamespace + '/anonymous'
        };

        Address address;            ///< ReplyTo address
    };

    /// To element of SOAP header
    struct To
    {
        std::string value;          ///< To address

        // attributes
        int mustUnderstand = 1;     ///< If set, receiver must understand this element
    };

    Action action;          ///< Action element
    MessageID messageID;    ///< MessageID element
    ReplyTo replyTo;        ///< ReplyTo element
};

/// SOAP message body
struct Body
{

};

/// SOAP Envelope element
struct Envelope
{
    Header header;  ///< Header element
    Body body;      ///< Body element
};

} // namespace soaplib
