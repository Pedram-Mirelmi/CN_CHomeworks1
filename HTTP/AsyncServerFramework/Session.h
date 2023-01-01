#pragma once
#include <asio.hpp>
#include <iostream>
#include <mutex>

#include "./io/BasicHTTPMessage.h"
#include "./NetMessages/HTTP-Request.hpp"

using std::string;

class Session : public std::enable_shared_from_this<Session>
{
    using socket = asio::ip::tcp::socket;


    socket m_socket;
    asio::streambuf m_inStreamBuff;
    std::vector<char> m_headerInBuffer;
    std::vector<char> m_bodyInBuffer;
    shared_ptr<HTTPMessage> m_tempMessage;
public:

    Session(socket&& socket)
        :m_socket(std::move(socket))
    {
    }

    Session(const Session& other) = delete;
    Session& operator=(const Session& other) = delete;

    ~Session()
    {
        // TODO log that the session is closing
        m_socket.close();
    }


public:

    inline socket& getSocket()
    {
        return m_socket;
    }
    inline std::vector<char>& getHeaderInBuffer()
    {
        return m_headerInBuffer;
    }

    inline std::vector<char>& getBodyInBuffer()
    {
        return m_bodyInBuffer;
    }

    void deserializeHeader()
    {
        auto headerReadSoFar = (char*)m_inStreamBuff.data().data();
        auto extraPoint = strstr(headerReadSoFar, "\r\n\r\n");

        m_headerInBuffer = std::vector<char>(headerReadSoFar, extraPoint);
        m_inStreamBuff.consume(m_headerInBuffer.size() + 4);

        if(!strcmp("GET", m_headerInBuffer.data()))
        {
            m_tempMessage = std::make_shared<HTTPRequest>();
            m_tempMessage->deserialize(m_headerInBuffer.data());
            m_headerInBuffer.clear();
        }
    }

    inline asio::streambuf &getInStreamBuff()
    {
        return m_inStreamBuff;
    }

    inline shared_ptr<HTTPMessage> getTempMessage() const
    {
        return m_tempMessage;
    }

};



