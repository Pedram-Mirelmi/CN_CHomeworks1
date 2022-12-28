#pragma once
#include <exception>
#include "../io/INetMessageBodyDeserializer.h"
#include "./NetMessages.h"
using std::make_shared;

class MessageBodyDeserializer : public INetMessageBodyDeserializer<DemoNetMessageType>
{
public:

    // INetMessageBodyDeserializer interface
public:
    virtual shared_ptr<NetMessage<DemoNetMessageType>> deserializeBody(NetMessageHeader<DemoNetMessageType> header, const char *bodyBuffer) override
    {
        switch (header.getMessageType())
        {
            case DemoNetMessageType::TEXT_MESSAGE:
                return shared_ptr<NetMessage<DemoNetMessageType>>(new SimpleTextMessage(std::string(bodyBuffer, header.getBodySize())));

            default:
                throw std::runtime_error("Unknown type of net message");
        }
    }
};
