#pragma once

#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"

class SendMessage : public _BNetMsg
{

    _UserId_T m_userId;
    std::string m_message;

    // ISerializable interface
public:
    SendMessage() = default;
    SendMessage(_UserId_T userId, std::string &&message)
        : _BNetMsg(MessageTypes::SEND, sizeof (m_userId) + message.size()),
          m_userId(std::move(userId)),
          m_message(std::move(message))
    {}
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::deserializePrimitiveType<_UserId_T>(buffer, m_userId);

        m_message.resize(m_header.getBodySize() - sizeof(_UserId_T));
        ISerializable::deserializeByteArray(buffer, m_message.data(), m_message.size(), false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializePrimitiiveType<_UserId_T>(buffer, m_userId);

        ISerializable::serializeByteArray(buffer, m_message.data(), m_message.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize()
               + sizeof (_UserId_T)
               + m_message.size();
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::SEND;
    }
    _UserId_T getUserId() const
    {
        return m_userId;
    }
    const std::string &getMessage() const
    {
        return m_message;
    }
};






