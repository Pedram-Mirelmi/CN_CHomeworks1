#pragma once

#include "./AsyncNetFramework/AbstractNetIOManager.h"
#include "NetMessages/FTP-Message-types.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"

class HelpContentMessage : BasicNetMessage<FTPMessageType>
{
protected:
    std::string m_helpContent;
    // ISerializable interface
    typedef uint16_t _helpContentSize_T  ;
public:
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        _helpContentSize_T contentSize;
        ISerializable::deserializePrimitiveType<_helpContentSize_T>(buffer, contentSize);

        m_helpContent.resize(contentSize);
        ISerializable::deserializeByteArray(buffer, m_helpContent.data(), contentSize, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_helpContentSize_T>(buffer, static_cast<_helpContentSize_T>(m_helpContent.size()));

        ISerializable::serializeByteArray(buffer, m_helpContent.data(), m_helpContent.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<FTPMessageType>::getHeaderSize()
               + sizeof(_helpContentSize_T)
               + m_helpContent.size();
    }

    // BasicNetMessage interface
public:
    const FTPMessageType &getMessageType() const override
    {
        return FTPMessageType::HELP_CONTENT;
    }
};
