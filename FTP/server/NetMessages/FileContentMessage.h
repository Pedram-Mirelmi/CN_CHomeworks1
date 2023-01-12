#pragma once

#include "../AsyncServerFramework/AbstractNetIOManager.h"
#include "./NetMessages/AllNetMessages.hpp"
#include "../typedefs.hpp"


class FileContentMessage : public _BNetMsg
{
    typedef uint32_t _fileSize_T ;
protected:
    std::vector<char> m_fileContent;
    // ISerializable interface
public:
    FileContentMessage() = default;
    FileContentMessage(std::vector<char>& content)
        :_BNetMsg(),
         m_fileContent(std::move(content))
    {
        m_header = _Header(NetMessageType::FILE_CONTENT, calculateNeededSizeForThis() - _Header::getHeaderSize());
    }
public:
    void deserialize(char *buffer) override
    {
        _fileSize_T contentSize;
        ISerializable::deserializePrimitiveType<_fileSize_T>(buffer, contentSize);

        m_fileContent.resize(contentSize);
        ISerializable::deserializeByteArray(buffer, m_fileContent.data(), contentSize, false);
    }
    void serialize(char *buffer) const override
    {
        m_header.serialize(buffer);
        buffer += NetMessageHeader<NetMessageType>::getHeaderSize();

        ISerializable::serializePrimitiiveType<_fileSize_T>(buffer, static_cast<_fileSize_T>(m_fileContent.size()));

        ISerializable::serializeByteArray(buffer, m_fileContent.data(), m_fileContent.size(), false);
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return NetMessageHeader<NetMessageType>::getHeaderSize()
               + sizeof(_fileSize_T)
               + m_fileContent.size();
    }

    std::vector<char> get_file_content() {
        return this->m_fileContent;
    }

    // BasicNetMessage interface
public:
    const NetMessageType &getMessageType() const override
    {
        return NetMessageType::FILE_CONTENT;
    }
public:
    const std::vector<char>& getFileContent() const
    {
        return m_fileContent;
    }
};
