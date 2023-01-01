#pragma once

#include "./AsyncServerFramework/io/BasicHTTPMessage.h"

class HTTPRequest : public HTTPMessage
{
    std::string m_uri;
    float m_httpMethod;

    // HTTPMessage interface
public:
    Type getMessageType() override
    {
        return Type::HTTP_REQUEST;
    }
};
