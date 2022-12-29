#pragma once
#include <string>
#include <cstring>

#include "../AsyncNetFramework/io/BasicNetMessage.h"

#include "./FTP-Message-types.h"


class UploadFileMessage : public BasicNetMessage<FTPMessageType>
{
    typedef uint16_t _filenameSize_T ;
protected:
    std::string m_filename;

    // ISerializable interface
public:
    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += NetMessageHeader<FTPMessageType>::getHeaderSize();

        _filenameSize_T filenameSize;
        ISerializable::deserializePrimitiveType<_filenameSize_T>(buffer, filenameSize);

        m_filename.resize(filenameSize);
        ISerializable::deserializeByteArray(buffer, m_filename.data(), filenameSize, false);
    }
    void serialize(char *buffer) const override
    {
        // serialize header
        m_header.serialize(buffer);
        buffer += m_header.getHeaderSize();

        // serialize filename string length
        ISerializable::serializePrimitiiveType<_filenameSize_T>(buffer, static_cast<_filenameSize_T>(m_filename.size()));

        ISerializable::serializeByteArray(buffer, m_filename.data(), m_filename.size());
    }
    uint32_t calculateNeededSizeForThis() const override
    {
        return m_header.getHeaderSize() // header
               + sizeof (_filenameSize_T) // length of the filename in a 8-bit unsigned int ( must be under 256 character)
               + m_filename.size(); // the username bytes
    }

    // BasicNetMessage interface
public:
    const FTPMessageType& getMessageType() const override
    {
        return FTPMessageType::USERNAME;
    }
};
