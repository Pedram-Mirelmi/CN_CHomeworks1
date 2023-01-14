#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class QuitMessage : public _BNetMsg
{
    // ISerializable interface
public:
    QuitMessage()
        :_BNetMsg(NetMessageType::QUIT, 0)
    {}
public:

    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<NetMessageType>::getHeaderSize();
    }

    // BasicNetMessage interface
public:
    const NetMessageType &getMessageType() const override
    {
        return NetMessageType::QUIT;
    }
};

