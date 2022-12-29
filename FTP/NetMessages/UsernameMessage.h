#pragma once
#include <string>
#include <cstring>

#include "../AsyncNetFramework/io/BasicNetMessage.h"

#include "./FTP-Message-types.h"


class PasswordMessage : public BasicNetMessage<FTPMessageType>
{
protected:
    std::string m_username;

    // ISerializable interface
public:
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);

    }
    void serialize(char *buffer) const override
    {
        // deserialize header
        m_header.serialize(buffer);
        buffer += m_header.getHeaderSize();

        // deserialize username size
        uint8_t usernameSize = (uint8_t)m_username.size();
        *(reinterpret_cast<uint8_t*>(buffer)) = usernameSize;
        buffer += sizeof(usernameSize);

        // deserialize username
        memcpy(buffer, m_username.data(), usernameSize);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return m_header.getHeaderSize() // header
               + sizeof (uint8_t) // length of the username in a 8-bit unsigned int ( must be under 256 character)
               + m_username.size(); // the username bytes
    }

    // BasicNetMessage interface
public:
    const FTPMessageType& getMessageType() const override
    {
        return FTPMessageType::USERNAME;
    }
};
