#pragma once

#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"


class UserInfoReplyMessage : public _BNetMsg
{

    std::string m_username;
    // ISerializable interface
public:
    UserInfoReplyMessage() = default;
    UserInfoReplyMessage(std::string &&username)
        : _BNetMsg(MessageTypes::INFOREPLY, username.size()),
          m_username(std::move(username))
    {}
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        m_username.resize(m_header.getBodySize());
        ISerializable::deserializeByteArray(buffer, m_username.data(), m_header.getBodySize(), false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializeByteArray(buffer, m_username.data(), m_username.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize()
               + m_username.size();
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::INFOREPLY;
    }
};


