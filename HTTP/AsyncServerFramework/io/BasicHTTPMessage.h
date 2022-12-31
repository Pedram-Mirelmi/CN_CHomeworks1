#pragma once
#include <unordered_map>
#include <vector>
#include <sstream>
#include "./ISerializable.h"

class HTTPMessage : public ISerializable,
                    public std::enable_shared_from_this<HTTPMessage>
{
    typedef std::unordered_map<std::string, std::string> _ssHashMap_;

    std::string m_startLine;
    _ssHashMap_ m_headers;
    std::vector<char> m_body;
public:
    HTTPMessage() = default;
    HTTPMessage(std::string&& startLine, _ssHashMap_&& headers, std::vector<char> body)
        : m_startLine(std::move(startLine)),
          m_headers(std::move(headers)),
          m_body(std::move(body))
    {}

    // ISerializable interface
public:
    void deserialize(char *buffer) override // buffer will be zero terminated since it's a string
    {
        auto buffString = std::string(buffer);

        size_t start_header_point;
        size_t header_body_point;

        start_header_point = buffString.find("\r\n");
        header_body_point = buffString.find("\r\n\r\n");

        m_startLine = buffString.substr(0, start_header_point);

        parsHeaders(buffString.substr(start_header_point + 2, header_body_point - (start_header_point + 2)));

        m_body.resize(buffString.size() - header_body_point - 4);
        memcpy(m_body.data(), buffString.data() + header_body_point+4, m_body.size());
    }

    void serialize(char *buffer) const override
    {
        ISerializable::serializeByteArray(buffer, m_startLine.data(), m_startLine.size());

        for (auto& pair : m_headers)
        {
            ISerializable::serializeByteArray(buffer, pair.first.data(), pair.first.size());
            ISerializable::serializeByteArray(buffer, ": ", 2);
            ISerializable::serializeByteArray(buffer, pair.second.data(), pair.second.size());
            ISerializable::serializeByteArray(buffer, "\r\n", 2);
        }
    }

    void parsHeaders(std::string&& headersString)
    {
        // Headers are not important for now!
//        std::stringstream stream(std::move(headersString));
//        std::string line;
//        while (getline(stream, line))
//        {

//        }
    }

    uint32_t calculateNeededSizeForThis() const override
    {
        uint32_t size = 0;
        size += m_startLine.size();
        for(auto& pair : m_headers)
            size += pair.first.size() + pair.second.size();

        size += m_body.size();
        return size;
    }
};


