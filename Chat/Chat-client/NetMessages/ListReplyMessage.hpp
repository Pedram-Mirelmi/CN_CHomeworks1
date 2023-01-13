#pragma once

#include <vector>
#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"

class ListReplyMessage : public _BNetMsg
{

    std::vector<_UserId_T> m_users;
    // ISerializable interface
public:
    ListReplyMessage() = default;
    ListReplyMessage(std::vector<_UserId_T> &&users)
        : _BNetMsg(MessageTypes::LISTREPLY, users.size() * sizeof(_UserId_T)),
          m_users(std::move(users))
    {}

    ListReplyMessage(const std::vector<_UserId_T> &users)
        : _BNetMsg(MessageTypes::LISTREPLY, users.size() * sizeof(_UserId_T)),
          m_users(users)
    {}

    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::deserializeByteArray(buffer,
                                            (char*)m_users.data(),
                                            m_users.size() * sizeof(_UserId_T),
                                            false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializeByteArray(buffer, (char*)m_users.data(), m_users.size() * sizeof (_UserId_T));

    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize() + m_users.size() * sizeof (_UserId_T);
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::LISTREPLY;
    }
    const std::vector<_UserId_T> &getUsers() const
    {
        return m_users;
    }
};




