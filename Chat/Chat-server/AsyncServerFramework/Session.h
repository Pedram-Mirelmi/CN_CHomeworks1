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

    std::vector<char> m_messageInBuffer;

    NetMessageHeader<MsgType> m_tempHeader;
public:

    Session(socket&& socket)
        :m_socket(std::move(socket))
    {
        m_messageInBuffer.resize(NetMessageHeader<MsgType>::getHeaderSize());
        m_id = 0;
    }

    Session(const Session& other) = delete;
    Session& operator=(const Session& other) = delete;

    ~Session()
    {
        // TODO log that the session is closing
        m_socket.close();
    }

    virtual void deserializeHeader()
    {
        // deserialize just header
        m_tempHeader.deserialize(m_messageInBuffer.data());

        m_messageInBuffer.resize(m_tempHeader.getBodySize() + NetMessageHeader<MsgType>::getHeaderSize());
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

    std::vector<char> getMessageInBuffer() {
        return m_messageInBuffer;
    }

    const NetMessageHeader<MsgType> &getTempHeader()
    {
        return m_tempHeader;
    }

    void resetBuffers()
    {
        m_messageInBuffer.resize(NetMessageHeader<MsgType>::getHeaderSize());
    }

};

