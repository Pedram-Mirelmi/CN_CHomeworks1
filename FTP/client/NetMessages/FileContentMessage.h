#pragma once

#include "./AsyncNetFramework/AbstractNetIOManager.h"
#include "NetMessages/FTP-Message-types.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "./IResponseResolver.h"
#include "../typedefs.hpp"


class FileContentMessage : public BasicNetMessage<FTPMessageType>
{
    typedef uint32_t _fileSize_T ;
protected:
    std::vector<char> m_fileContent;
    // ISerializable interface
public:
    FileContentMessage() = default;
    FileContentMessage(std::vector<char>&& content)
        :_BNetMsg(),
         m_fileContent(std::move(content))
    {
        m_header = _Header(FTPMessageType::FILE_CONTENT, calculateNeededSizeForThis() - _Header::getHeaderSize());
    }
public:
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        _fileSize_T contentSize;
        ISerializable::deserializePrimitiveType<_fileSize_T>(buffer, contentSize);

        m_fileContent.resize(contentSize);
        ISerializable::deserializeByteArray(buffer, m_fileContent.data(), contentSize, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_fileSize_T>(buffer, static_cast<_fileSize_T>(m_fileContent.size()));

        ISerializable::serializeByteArray(buffer, m_fileContent.data(), m_fileContent.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<FTPMessageType>::getHeaderSize()
               + sizeof(_fileSize_T)
               + m_fileContent.size();
    }

    // BasicNetMessage interface
public:
    FTPMessageType getMessageType() const override
    {
        return FTPMessageType::FILE_CONTENT;
    }
public:
    const std::vector<char>& getFileContent() const
    {
        return m_fileContent;
    }
};
