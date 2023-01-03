#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "FTP-Message-types.h"
#include "./NetMessages/AllNetMessages.hpp"

#include "../typedefs.hpp"

class QuitMessage : public BasicNetMessage<FTPMessageType>
{
    // ISerializable interface
public:
    QuitMessage()
        :_BNetMsg(FTPMessageType::QUIT, 0)
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
        return NetMessageHeader<FTPMessageType>::getHeaderSize();
    }

    // BasicNetMessage interface
public:
    const FTPMessageType &getMessageType() const override
    {
        return FTPMessageType::QUIT;
    }
};

