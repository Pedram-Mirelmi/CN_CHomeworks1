#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class ShortResponseMessage : public _BNetMsg
{
    typedef u_int16_t _responseNum_T ;
protected:
    uint16_t m_responseNumber;
    // ISerializable interface

public:
    ShortResponseMessage() = default;
    ShortResponseMessage(uint16_t responseNumber)
        : _BNetMsg(NetMessageType::SHORT_RESPONSE, calculateNeededSizeForThis() - _Header::getHeaderSize()),
          m_responseNumber(std::move(responseNumber))

    {}

    void deserialize(char *buffer) override
    {
        ISerializable::deserializePrimitiveType<_responseNum_T>(buffer, m_responseNumber, false);
    }

    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<NetMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_responseNum_T>(buffer, static_cast<_responseNum_T>(m_responseNumber), false);
    }

    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<NetMessageType>::getHeaderSize()
               + sizeof (_responseNum_T);
    }

    // BasicNetMessage interface
public:
    const NetMessageType &getMessageType() const override
    {
        return NetMessageType::SHORT_RESPONSE;
    }

    uint16_t getResponseNumber() const
    {
        return m_responseNumber;
    }
};





