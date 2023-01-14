#pragma once
#include <string>
#include <cstring>

#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class PasswordMessage : public _BNetMsg
{
    typedef uint8_t _passwordSize_T;
protected:
    std::string m_password;

    // ISerializable interface
public:
    PasswordMessage() = default;
    PasswordMessage(const std::string& password)
        : _BNetMsg(NetMessageType::PASSWORD, password.size()),
          m_password(password)
    {}

    void deserialize(char *buffer) override
    {
        std::cout << "password: " << buffer << endl;
        _passwordSize_T passwordSize;
        ISerializable::deserializePrimitiveType<_passwordSize_T>(buffer, passwordSize);

        m_password.resize(passwordSize);
        ISerializable::deserializeByteArray(buffer, m_password.data(), passwordSize, true);
    }
    void serialize(char *buffer) const override
    {
        // deserialize header
        m_header.serialize(buffer);
        buffer += m_header.getHeaderSize();

        ISerializable::serializePrimitiiveType<_passwordSize_T>(buffer, static_cast<_passwordSize_T>(m_password.size()));

        ISerializable::serializeByteArray(buffer, m_password.data(), m_password.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return m_header.getHeaderSize() // header
               + sizeof (_passwordSize_T) // length of the password in a 8-bit unsigned int ( must be under 256 character)
               + m_password.size(); // the username bytes
    }
    
    std::string get_password() {
        return this->m_password;
    }

    // BasicNetMessage interface
public:
    const NetMessageType& getMessageType() const override
    {
        return NetMessageType::PASSWORD;
    }
};
