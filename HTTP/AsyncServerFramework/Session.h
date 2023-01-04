#pragma once
#include <asio.hpp>
#include <iostream>
#include <mutex>

#include "./io/BasicHTTPMessage.h"
#include "./io/HTTP-Request.hpp"

using std::string;

class Session : public std::enable_shared_from_this<Session>
{
    using socket = asio::ip::tcp::socket;


    socket m_socket;
    asio::streambuf m_inStreamBuff;
    shared_ptr<HTTPRequest> m_tempRequest;
public:

    Session(socket&& socket)
        :m_socket(std::move(socket))
    {
        m_tempRequest = std::make_shared<HTTPRequest>();
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

    void deserializeHeader(std::size_t bytesTransfered)
    {
        auto headerReadSoFar = (char*)m_inStreamBuff.data().data();

        std::stringstream stream;
        stream.write(headerReadSoFar, bytesTransfered);

        m_tempRequest->deserialize(stream);
        m_inStreamBuff.consume(m_tempRequest->calculateNeededSizeForThis() + 4);
    }

    inline asio::streambuf &getInStreamBuff()
    {
        return m_inStreamBuff;
    }

    inline shared_ptr<HTTPRequest> getTempRequest() const
    {
        return m_tempRequest;
    }

};



