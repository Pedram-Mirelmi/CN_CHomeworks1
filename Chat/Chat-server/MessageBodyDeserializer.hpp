#pragma once

#include "./NetMessages/AllNetMessages.hpp"
#include "./AsyncServerFramework/io/INetMessageDeserializer.h"

class MessageBodyDeserializer : public INetMessageDeserializer<MessageTypes>
{


    // INetMessageBodyDeserializer interface
public:
    shared_ptr<NetMessage<MessageTypes>>
    deserializeMessage(NetMessageHeader<MessageTypes> header,
                    char *buffer) override
    {
        _BNetMsg* msg;
        switch (header.getMessageType()) {
            case MessageTypes::CONNECT:
                msg = new ConnectMessage();
                break;
            case MessageTypes::CONNACK:
                msg = new ConnAckMessage();
                break;
            case MessageTypes::LIST:
                msg = new ListMessage();
                break;
            case MessageTypes::LISTREPLY:
                msg = new ListReplyMessage();
                break;
            case MessageTypes::INFO:
                msg = new UserInfoMessage();
                break;
            case MessageTypes::INFOREPLY:
                msg = new UserInfoReplyMessage();
                break;
            case MessageTypes::SEND:
                msg = new SendMessage();
                break;
            case MessageTypes::SENDREPLY:
                msg = new SendReplyMessage();
                break;
            case MessageTypes::RECEIVE:
                msg = new ReceiveMessage();
                break;
            case MessageTypes::RECEIVEREPLY:
                msg = new ReceiveReplyMessage();
                break;
        }
        msg->deserialize(buffer);
        return shared_ptr<_BNetMsg>(msg);
    }
};
