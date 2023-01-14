#pragma once

#include "./ChatServerMessagesTypes.hpp"
#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../ChatServer-typedefs.hpp"

class ConnAckMessage : public _BNetMsg
{
    // ISerializable interface
public:
    ConnAckMessage()
        : _BNetMsg(MessageTypes::CONNACK, calculateNeededSizeForThis())
    {

    }
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
        return MessageTypes::CONNACK;
    }
};
