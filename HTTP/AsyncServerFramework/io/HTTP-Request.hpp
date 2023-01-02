#pragma once

#include "./AsyncServerFramework/io/BasicHTTPMessage.h"

class HTTPRequest : public HTTPMessage
{
public:
    enum RequestMethod {POST, GET, PUT, PATCH, DELETE};
private:
    std::string m_uri;
    RequestMethod m_requestMethod;


    // HTTPMessage interface
public:
    Type getMessageType() override
    {
        return Type::HTTP_REQUEST;
    }

    RequestMethod getRequestType()
    {
        return m_requestMethod;
    }

    const std::string &getUri() const
    {
        return m_uri;
    }
    // ISerializable interface
public:
    void deserialize(char *buffer) override
    {
        HTTPMessage::deserialize(buffer);
        parseStartLine();
    }

private:
    void parseStartLine()
    {
        std::stringstream stream(m_startLine);
        std::string reqMethod, httpMethod;
        stream >> reqMethod;
        setRequestMethod(std::move(reqMethod));
        stream >> m_uri;
    }
    void setRequestMethod(const std::string& methodStr)
    {
        if(methodStr == "GET")
            m_requestMethod = GET;
        else if(methodStr == "POST")
            m_requestMethod = POST;
        else if(methodStr == "PUT")
            m_requestMethod = PUT;
        else if(methodStr == "PATCH")
            m_requestMethod = PATCH;
        else if(methodStr == "DELETE")
            m_requestMethod = DELETE;
    }
};


