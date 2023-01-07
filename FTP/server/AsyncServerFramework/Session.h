#pragma once
#include <asio.hpp>
#include <iostream>
#include <mutex>

#include "io/BasicNetMessage.h"
#include "./io/BasicNetMessage.h"
#include "Config.h"

using std::string;

template <typename MsgType>
class Session : public std::enable_shared_from_this<Session<MsgType>>
{
    using socket = asio::ip::tcp::socket;

    // user information variables
    User user;
    bool is_user_name_set = false;
    bool is_authenticated = false;


    socket m_socket;
    std::vector<char> m_headerInBuffer;
    std::vector<char> m_bodyInBuffer;
    NetMessageHeader<MsgType> m_tempHeader;
public:
    Session(socket&& socket)
        :m_socket(std::move(socket))
    {
        m_headerInBuffer.resize(sizeof(NetMessageHeader<MsgType>));
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
        m_tempHeader.deserialize(m_headerInBuffer.data());
        m_bodyInBuffer.resize(m_tempHeader.getBodySize());
    }

public:

    socket& getSocket()
    {
        return m_socket;
    }
    char *getHeaderInBuffer()
    {
        return m_headerInBuffer.data();
    }
    char *getBodyInBuffer()
    {
        return m_bodyInBuffer.data();
    }

    NetMessageHeader<MsgType> &getTempHeader()
    {
        return m_tempHeader;
    }

    // user information functions

    void set_user(User& user) {
        this->user = user;
        this->is_user_name_set = true;
    }

    bool check_password(const string password) {
        if(this->user.password == password) {
            this->is_authenticated = true;
            return true;
        }
        return false;
    }


};

