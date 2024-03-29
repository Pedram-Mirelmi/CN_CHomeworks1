#pragma once
#include <string>
#include <cstring>

#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class UsernameMessage : public _BNetMsg
{
protected:
    std::string m_username;

    // ISerializable interface
public:
    UsernameMessage() = default;
    UsernameMessage(const std::string& username)
        :_BNetMsg(NetMessageType::USERNAME, calculateNeededSizeForThis() - _Header::getHeaderSize()),
         m_username(username)
    {}

    void deserialize(char *buffer) override
    {
        uint8_t usernameSize;
        ISerializable::deserializePrimitiveType<uint8_t>(buffer, usernameSize);
        m_username.resize(usernameSize);

        ISerializable::deserializeByteArray(buffer, m_username.data(), usernameSize, true);
    }
    void serialize(char *buffer) const override
    {
        // deserialize header
        m_header.serialize(buffer);
        buffer += m_header.getHeaderSize();

        // deserialize username size
        ISerializable::serializePrimitiiveType<uint8_t>(buffer, (uint8_t)m_username.size());

        // deserialize username
        ISerializable::serializeByteArray(buffer, m_username.data(), m_username.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return m_header.getHeaderSize() // header
               + sizeof (uint8_t) // length of the username in a 8-bit unsigned int ( must be under 256 character)
               + m_username.size(); // the username bytes
    }

    std::string get_username(){
        return this->m_username;
    }

    // BasicNetMessage interface
public:
    const NetMessageType& getMessageType() const override
    {
        return NetMessageType::USERNAME;
    }
};
