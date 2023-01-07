#pragma once
#include <exception>
#include "AsyncServerFramework/io/INetMessageBodyDeserializer.h"
#include "NetMessages/AllNetMessages.hpp"
#include "typedefs.hpp"
using std::make_shared;
using std::shared_ptr;

class MessageBodyDeserializer : public INetMessageBodyDeserializer<NetMessageType>
{
public:     

    // INetMessageBodyDeserializer interface
public:
    virtual shared_ptr<NetMessage<NetMessageType>> deserializeBody(NetMessageHeader<NetMessageType> header, char *bodyBuffer) override
    {
        // std::cout << "message type is: ";
        switch (header.getMessageType())
        {
            case NetMessageType::USERNAME:
            {
                // std::cout << "USERNAME\n";
                // std::cout << "body: " << bodyBuffer << '\n';
                // std::cout << "message: " <<  message->get_username() << "\n";
                auto message = shared_ptr<UsernameMessage>(new UsernameMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);
                break;
            }
            case NetMessageType::PASSWORD:
            {
                auto message = shared_ptr<PasswordMessage>(new PasswordMessage());
                message->deserialize(bodyBuffer);
                return shared_ptr<_BNetMsg>(message);   
                break;
            }
            default:
                throw std::runtime_error("Unknown type of net message");
        }
    }
};
