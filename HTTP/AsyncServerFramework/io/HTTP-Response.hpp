#pragma once
#include <fmt/format.h>
#include "AsyncServerFramework/io/BasicHTTPMessage.h"

class HTTPResponse : public HTTPMessage
{
    uint16_t m_statusCode;



    // HTTPMessage interface
public:
    HTTPResponse(uint16_t statusCode, _ssHashMap_&& headers, std::vector<char>&& body)
        : HTTPMessage(),
          m_statusCode(std::move(statusCode))
    {
        m_headers = std::move(headers);
        m_body = std::move(body);
        m_startLine = fmt::format("HTTP/1.1 {} OK", statusCode);
    }
    Type getMessageType() override
    {
        return Type::HTTP_RESPONSE;
    }

    // ISerializable interface
public:
    void serialize(stringstream& stream) const override
    {
        HTTPMessage::serialize(stream);

        if(!m_body.empty())
            ISerializable::serializeByteArray(stream, m_body.data(), m_body.size());

    }
};


