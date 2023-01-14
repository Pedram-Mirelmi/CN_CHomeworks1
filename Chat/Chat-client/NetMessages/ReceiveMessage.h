#pragma once

#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"

class ReceiveMessage : public _BNetMsg
{


    // ISerializable interface
public:
    ReceiveMessage()
        :_BNetMsg(MessageTypes::RECEIVE, calculateNeededSizeForThis() - _Header::getHeaderSize())
    {

    };
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
