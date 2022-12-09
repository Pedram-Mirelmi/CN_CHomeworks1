#pragma once
#include "../io/INetMessageBodyDeserializer.h"
#include "./NetMessages.h"
using std::make_shared;

class MessageBodyDeserializer : INetMessageBodyDeserializer<DemoNetMessageType>
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

        }
    }
};
