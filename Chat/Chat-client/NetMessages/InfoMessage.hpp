#pragma once

#include "./MessagesTypes.hpp"
#include "../AsyncNetFramework/io/BasicNetMessage.h"
#include "../typedefs.hpp"


class UserInfoMessage : public _BNetMsg
{


    // ISerializable interface
    _UserId_T m_wantedUserId;

public:
    UserInfoMessage() = default;
    UserInfoMessage(_UserId_T wantedUserId)
        : _BNetMsg(MessageTypes::INFO, sizeof (_UserId_T)),
          m_wantedUserId(std::move(wantedUserId))
    {}
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();


        ISerializable::deserializePrimitiveType<_UserId_T>(buffer, m_wantedUserId, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializePrimitiiveType<_UserId_T>(buffer, m_wantedUserId, false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize()
               + sizeof (_UserId_T);
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::INFO;
    }
    _UserId_T getWantedUserId() const
    {
        return m_wantedUserId;
    }
};




