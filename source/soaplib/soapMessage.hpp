#pragma once

#include <string>

namespace soaplib {

struct Header
{
    struct Action
    {
        std::string value;

        // attributes
        int mustUnderstand = 1;
    };

    struct MessageID
    {
        std::string value;  // urn
    };

    struct ReplyTo
    {
        struct Address
        {
            std::string value;  // addressNamespace + '/anonymous'
        };

        Address address;
    };

    struct To
    {
        std::string value;

        // attributes
        int mustUnderstand = 1;
    };

    Action action;
    MessageID messageID;
    ReplyTo replyTo;
};

struct Body
{

};

struct Envelope
{
    Header header;
    Body body;
};

} // namespace soaplib
