#pragma once

#include "./AsyncNetFramework/AbstractNetIOManager.h"
#include "NetMessages/FTP-Message-types.h"
#include "./IResponseResolver.h"


class ShortResponseMessage : BasicNetMessage<FTPMessageType>
{
    typedef u_int16_t _responseNum_T ;
protected:
    uint16_t m_responseNumber;
    // ISerializable interface
public:
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        ISerializable::deserializePrimitiveType<_responseNum_T>(buffer, m_responseNumber, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_responseNum_T>(buffer, static_cast<_responseNum_T>(m_responseNumber), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<FTPMessageType>::getHeaderSize()
               + sizeof (_responseNum_T);
    }

    // BasicNetMessage interface
public:
    const FTPMessageType &getMessageType() const override
    {
        return FTPMessageType::SHORT_RESPONSE;
    }
};

