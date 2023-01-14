#pragma once
#include <string>
#include "./ChatServerMessagesTypes.hpp"
#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../ChatServer-typedefs.hpp"

class ConnectMessage : public _BNetMsg // NetMessage<MessageTpyes>
{

private:
    std::string m_name;
    // ISerializable interface
public:
    ConnectMessage() = default;
    ConnectMessage(std::string &&name)
        : _BNetMsg(MessageTypes::CONNECT, name.size()),
          m_name(std::move(name))
    {}

    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += _Header::getHeaderSize();

        m_name.resize(m_header.getBodySize());
        ISerializable::deserializeByteArray(buffer, m_name.data(), m_header.getBodySize(), false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += _Header::getHeaderSize();

        ISerializable::serializeByteArray(buffer, m_name.data(), m_name.size(), false);

    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return _Header::getHeaderSize() + m_name.size();
    }

    // NetMessage interface
public:
    MessageTypes getMessageType() const override
    {
        return MessageTypes::CONNECT;
    };
    const std::string &getName() const
    {
        return m_name;
    }
};




