#pragma once
#include <asio.hpp>
#include <iostream>
#include <mutex>
#include <cctype>

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
    bool next_upload = false;               // next message is upload
    string file_name;                       // file to upload name

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
    bool is_autherized() {
        return this->is_authenticated;
    }

    void set_user(User& user) {
        this->user = user;
        this->is_user_name_set = true;
    }

    bool is_admin() {
        if(this->is_authenticated) 
            return this->user.is_admin;
        return false;
    }

    bool check_password(const string password) {

        if(this->user.password == password) {
            this->is_authenticated = true;
            return true;
        }
        return false;
    }

    bool quit() {
        this->is_authenticated = false;
        this->is_user_name_set = false;
        this->want_to_upload = false;
    }   

    void set_next_upload(){
        this->next_upload = true;
    }

    string get_file_name() {
        return this->file_name;
    }

    void set_file_name(string filename)
    {
        this->file_name = filename;
    }

    bool can_upload() {
        return this->next_upload;
    }

    bool download(int size) {
        if (this->user.remain_storage > size) {
            this->user.remain_storage -= size;
            return true;                            // download
        }       
        return false;                               // don't download
    }

    int get_client_port() {
        return m_socket.remote_endpoint().port();
    }

};

