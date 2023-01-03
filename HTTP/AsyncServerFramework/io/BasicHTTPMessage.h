#pragma once
#include <unordered_map>
#include <vector>
#include <sstream>
#include "./ISerializable.h"

class HTTPMessage : public ISerializable,
                    public std::enable_shared_from_this<HTTPMessage>
{

protected:
    typedef std::unordered_map<std::string, std::string> _ssHashMap_;
    std::string m_startLine;
    _ssHashMap_ m_headers;
    std::vector<char> m_body;

public:
    enum Type {HTTP_REQUEST, HTTP_RESPONSE};

    HTTPMessage() = default;

    HTTPMessage(std::string&& startLine, _ssHashMap_&& headers, std::vector<char>&& body)
        : m_startLine(std::move(startLine)),
          m_headers(std::move(headers)),
          m_body(std::move(body))
    {}

    // ISerializable interface
public:
    virtual void deserialize(char *buffer) override // buffer will be zero terminated since it's a string
    {
        char* start_header_point;
        char* header_body_point;

//        start_header_point = buffString.find("\r\n");
        start_header_point = strstr(buffer, "\r\n");
//        header_body_point = buffString.find("\r\n\r\n");
        header_body_point = strstr(buffer, "\r\n\r\n");

        m_startLine = std::string(buffer, start_header_point);

        parsHeaders(std::string(start_header_point + 2, header_body_point));

        if(hasBody()) // not in this project
        {
            m_body.resize(std::stoi(m_headers["Content-Length"]));
            memcpy(m_body.data(), header_body_point + 4, m_body.size());
        }
    }

    virtual void serialize(char *buffer) const override
    {
        auto backup = buffer;
        ISerializable::serializeByteArray(buffer, m_startLine.data(), m_startLine.size());
        ISerializable::serializeByteArray(buffer, "\r\n", 2);
        for (auto& pair : m_headers)
        {
            ISerializable::serializeByteArray(buffer, pair.first.data(), pair.first.size());
            ISerializable::serializeByteArray(buffer, ": ", 2);
            ISerializable::serializeByteArray(buffer, pair.second.data(), pair.second.size());
            ISerializable::serializeByteArray(buffer, "\r\n", 2);
        }
        ISerializable::serializeByteArray(buffer, "\r\n\r\n", 4);
    }

    virtual void parsHeaders(std::string&& headersString)
    {
        std::stringstream headersStream(std::move(headersString));
        std::string line;
        while(getline(headersStream, line))
        {
            auto delimiter = line.find(":");
            auto key = line.substr(0, delimiter);
            auto value = line.substr(delimiter + 2, line.size()-delimiter-1);
            m_headers[key] = value;
        }
    }

    virtual uint32_t calculateNeededSizeForThis() const override
    {
        uint32_t size = 0;
        size += m_startLine.size();
        size += 2; // \r\n
        for(auto& pair : m_headers)
            size += pair.first.size() + 2 + pair.second.size() + 2;

        size += 4; // \r\n\r\n
        size += m_body.size();
        return size;
    }

    virtual inline bool hasBody() const
    {
        return !m_startLine.starts_with("GET");
    }

    inline const std::string &getStartLine() const
    {
        return m_startLine;
    }

    inline const _ssHashMap_ &getHeaders() const
    {
        return m_headers;
    }

    inline const std::vector<char>& getBody() const
    {
        return m_body;
    }

    virtual Type getMessageType() = 0;
};


