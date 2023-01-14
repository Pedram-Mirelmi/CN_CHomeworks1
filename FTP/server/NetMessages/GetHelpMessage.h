#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "../NetMessageTypes.h"
#include "AllNetMessages.hpp"
#include "../typedefs.hpp"

class GetHelpMessage : public _BNetMsg
{


    // ISerializable interface
public:
    GetHelpMessage()
        :_BNetMsg(NetMessageType::GET_HELP, 0)
    {}
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
        return NetMessageType::GET_HELP;
    }
};

