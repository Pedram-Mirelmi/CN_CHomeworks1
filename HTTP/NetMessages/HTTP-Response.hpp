#pragma once

#include "AsyncServerFramework/io/BasicHTTPMessage.h"

class HTTPResponse : public HTTPMessage
{
    uint16_t m_statusCode;



    // HTTPMessage interface
public:
    Type getMessageType() override
    {
        return Type::HTTP_RESPONSE;
    }

    // ISerializable interface
public:
    void deserialize(char *buffer) override;
    void serialize(char *buffer) const override;
};
