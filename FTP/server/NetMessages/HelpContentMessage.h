#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "AllNetMessages.hpp"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class HelpContentMessage : public _BNetMsg
{
protected:
    std::string m_helpContent = "......... Content of Help ........";
    // ISerializable interface
    typedef uint16_t _helpContentSize_T;
public:
    HelpContentMessage()
        :_BNetMsg()
    {
        m_header = _Header(NetMessageType::HELP_CONTENT, calculateNeededSizeForThis() - _Header::getHeaderSize());
    }
public:
    void deserialize(char *buffer) override
    {
        _helpContentSize_T contentSize;
        ISerializable::deserializePrimitiveType<_helpContentSize_T>(buffer, contentSize);

        m_helpContent.resize(contentSize);
        ISerializable::deserializeByteArray(buffer, m_helpContent.data(), contentSize, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<NetMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_helpContentSize_T>(buffer, static_cast<_helpContentSize_T>(m_helpContent.size()));

        ISerializable::serializeByteArray(buffer, m_helpContent.data(), m_helpContent.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<NetMessageType>::getHeaderSize()
               + sizeof(_helpContentSize_T)
               + m_helpContent.size();
    }

    // BasicNetMessage interface
public:
    const NetMessageType &getMessageType() const override
    {
        return NetMessageType::HELP_CONTENT;
    }
    const std::string &getHelpContent() const
    {
        return m_helpContent;
    }
};


