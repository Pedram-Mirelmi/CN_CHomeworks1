#pragma once

#include "./ChatServerMessagesTypes.hpp"
#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../ChatServer-typedefs.hpp"

class ReceiveMessage : public _BNetMsg
{


    // ISerializable interface
public:
    ReceiveMessage() = default;
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize();
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::RECEIVE;
    }
};
