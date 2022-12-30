#pragma once
#include <exception>
#include "AsyncServerFramework/io/INetMessageBodyDeserializer.h"
#include "./NetMessages.h"
using std::make_shared;

class MessageBodyDeserializer : public INetMessageBodyDeserializer<NetMessageType>
{
public:

    // INetMessageBodyDeserializer interface
public:
    virtual shared_ptr<NetMessage<NetMessageType>> deserializeBody(NetMessageHeader<NetMessageType> header, const char *bodyBuffer) override
    {
        switch (header.getMessageType())
        {
            case NetMessageType::USERNAME:
                return shared_ptr<NetMessage<NetMessageType>>(new SimpleTextMessage(std::string(bodyBuffer, header.getBodySize())));

            default:
                throw std::runtime_error("Unknown type of net message");
        }
    }
};
