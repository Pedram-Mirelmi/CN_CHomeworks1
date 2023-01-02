#pragma once

#include "./ChatServerMessagesTypes.hpp"
#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../ChatServer-typedefs.hpp"

class ReceiveReplyMessage : public _BNetMsg
{

    _UserId_T m_senderId;
    std::string m_message;
    // ISerializable interface
public:
    ReceiveReplyMessage() = default;
    ReceiveReplyMessage(_UserId_T senderId, std::string &&message)
        : _BNetMsg(MessageTypes::RECEIVEREPLY, sizeof(m_senderId) + message.size()),
          m_senderId(std::move(senderId)),
          m_message(std::move(message))
    {}

    ReceiveReplyMessage(_UserId_T senderId, const std::string &message)
        : _BNetMsg(MessageTypes::RECEIVEREPLY, sizeof(m_senderId) + message.size()),
          m_senderId(std::move(senderId)),
          m_message(std::move(message))
    {}

    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::deserializePrimitiveType<_UserId_T>(buffer, m_senderId);

        m_message.resize(m_header.getBodySize() - 2);
        ISerializable::deserializeByteArray(buffer, m_message.data(), m_header.getBodySize(), false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializePrimitiiveType<_UserId_T>(buffer, m_senderId);

        ISerializable::serializeByteArray(buffer, m_message.data(), m_message.size());
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
        return MessageTypes::RECEIVEREPLY;
    }
    _UserId_T getSenderId() const
    {
        return m_senderId;
    }
    const std::string &getMessage() const
    {
        return m_message;
    }
};






