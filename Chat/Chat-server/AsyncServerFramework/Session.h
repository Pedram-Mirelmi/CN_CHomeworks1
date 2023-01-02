#pragma once
#include <asio.hpp>
#include <iostream>
#include <mutex>

#include "io/BasicNetMessage.h"
#include "./io/BasicNetMessage.h"

using std::string;

template <typename MsgType>
class Session : public std::enable_shared_from_this<Session<MsgType>>
{
    using socket = asio::ip::tcp::socket;


    socket m_socket;
    uint64_t m_id;

    /*
     *   In a normal state:
     *
     *   message data:  [##########################################################]
     *                   ^                      ^
     *    m_messageBuffer=m_headerBuffer  m_bodyInBuffer
     */
    char* m_messageBuffer = nullptr;
    char* m_headerInBuffer = nullptr;
    char* m_bodyInBuffer = nullptr;

    NetMessageHeader<MsgType> m_tempHeader;
public:

    Session(socket&& socket)
        :m_socket(std::move(socket))
    {
        // first we allocate as mush as a header
        m_messageBuffer = m_headerInBuffer = new char[NetMessageHeader<MsgType>::getHeaderSize()];
        m_id = 0;
    }

    Session(const Session& other) = delete;
    Session& operator=(const Session& other) = delete;

    ~Session()
    {
        // TODO log that the session is closing
        m_socket.close();
        delete [] m_messageBuffer;
    }

    virtual void deserializeHeader()
    {
        // deserialize just header
        m_tempHeader.deserialize(m_headerInBuffer);
        m_messageBuffer = new char[NetMessageHeader<MsgType>::getHeaderSize()
                                   + m_tempHeader.getBodySize()];

        memcpy(m_messageBuffer, m_headerInBuffer, NetMessageHeader<MsgType>::getHeaderSize());
        delete [] m_headerInBuffer; // delete old header buffer
        m_headerInBuffer = m_messageBuffer;

        m_bodyInBuffer = m_messageBuffer + NetMessageHeader<MsgType>::getHeaderSize();
    }

    virtual void resetBuffers()
    {
        delete [] m_messageBuffer;
        m_messageBuffer = m_headerInBuffer = new char[NetMessageHeader<MsgType>::getHeaderSize()];
    }

public:
    inline uint64_t getId() const
    {
        return m_id;
    }

    inline void setId(uint64_t newId)
    {
        m_id = newId;
    }

    socket& getSocket()
    {
        return m_socket;
    }
    char *getHeaderInBuffer()
    {
        return m_headerInBuffer;
    }
    char *getBodyInBuffer()
    {
        return m_bodyInBuffer;
    }
    char *getMessageInBuffer()
    {
        return m_messageBuffer;;
    }

    const NetMessageHeader<MsgType> &getTempHeader()
    {
        return m_tempHeader;
    }

};

