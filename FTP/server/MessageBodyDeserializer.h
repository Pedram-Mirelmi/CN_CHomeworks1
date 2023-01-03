#pragma once
#include <exception>
#include "AsyncServerFramework/io/INetMessageBodyDeserializer.h"
#include "NetMessages/AllNetMessages.hpp"
#include "typedefs.hpp"
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
                return shared_ptr<_BNetMsg>(new UsernameMessage(std::string(bodyBuffer, header.getBodySize())));
                break;

            case NetMessageType::PASSWORD:
                break;
            default:
                throw std::runtime_error("Unknown type of net message");
        }
    }
};
