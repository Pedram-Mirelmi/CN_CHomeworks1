#pragma once

#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"

class SendReplyMessage : public _BNetMsg
{

    bool m_success;

    // ISerializable interface
public:
    SendReplyMessage() = default;
    SendReplyMessage(bool success)
        : _BNetMsg(MessageTypes::SENDREPLY, sizeof (bool)),
          m_success(success)
    {}
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::deserializePrimitiveType<bool>(buffer, m_success, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializePrimitiiveType<bool>(buffer, m_success, false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize()
               + sizeof (bool);
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::SENDREPLY;
    }
    bool getSuccess() const
    {
        return m_success;
    }
};




