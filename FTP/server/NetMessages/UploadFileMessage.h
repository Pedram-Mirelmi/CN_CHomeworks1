#pragma once
#include <string>
#include <cstring>

#include "../AsyncServerFramework/io/BasicNetMessage.h"
#include "../NetMessageTypes.h"
#include "../typedefs.hpp"

class UploadFileMessage : public _BNetMsg
{
    typedef uint16_t _filenameSize_T ;
protected:
    std::string m_filename;

    // ISerializable interface
public:
    UploadFileMessage() = default;
    UploadFileMessage(const std::string& filename)
        : _BNetMsg(NetMessageType::UPLOAD_FILE, calculateNeededSizeForThis() - _Header::getHeaderSize()),
          m_filename(filename)
    {}

    void deserialize(char *buffer) override
    {
        m_header.deserialize(buffer);
        buffer += NetMessageHeader<NetMessageType>::getHeaderSize();

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

    std::string get_file_name(){
        return this->m_filename;
    }

    // BasicNetMessage interface
public:
    const NetMessageType& getMessageType() const override
    {
        return NetMessageType::USERNAME;
    }
};
